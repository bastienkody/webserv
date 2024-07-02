#include"ConfigFile.hpp"
#include"Server.hpp"

//	constructors + copy
ConfigFile::ConfigFile() {}
ConfigFile::~ConfigFile() {}
ConfigFile::ConfigFile(const std::string filename) : _filename(filename) {}
ConfigFile::ConfigFile(const ConfigFile & src) {*this = src;}
ConfigFile & ConfigFile::operator=(const ConfigFile & rhs)
{
	if(this != &rhs)
	{
		this->_filename = rhs._filename;
		this->_rawData = rhs.getRawData();
		this->_servers = rhs.getServers();
	}
	return (*this);
}

//	printer
void	ConfigFile::printAll() const
{
	std::cout << "#### Config printer (server nb:" << getServers().size() << ") ####" << std::endl;
	for (std::vector<Server>::const_iterator it = getServers().begin(); it != getServers().end(); ++it)
		it->printAll();
	std::cout << "#### end of config ####" << std::endl;
}

//	Store filename into string _rawData
void	ConfigFile::openReadFileToStr()
{
	_fs.open(_filename.c_str(), std::fstream::in);
	if (_fs.fail())
		throw std::invalid_argument("Confgfile: " + _filename + " failed to open.");

	// test size vs string.max (cf. cpp01/04)
	_fs.seekg(0, std::ios_base::end);
	unsigned long dataSize = static_cast<unsigned long>(_fs.tellg());
	std::cout << "Size:" << dataSize << std::endl;
	_fs.seekg(std::ios_base::beg);
	std::string string = "test";
	if (dataSize == 0 || dataSize >= string.max_size())
		throw std::invalid_argument("Data from " + _filename + " is too big or empty");

	//	read into _rawData (pure string)
	char *buf = new char [dataSize + 1];
	buf[dataSize] = '\0';
	_fs.read(buf, dataSize);
	if (_fs)
		_rawData = buf;
	else
		throw std::invalid_argument("Problem with read on configfile");
	delete [] buf;
}

//	Read the whole config file line by line (erase comment + trim OWS) looking for server parts
void	ConfigFile::readAllInfos()
{
	std::string	raw(_rawData), line, rawLine;

	while (raw.find('\n') != std::string::npos)
	{
		rawLine = raw.substr(0, raw.find('\n'));
		line = ParserUtils::trimOWS(ParserUtils::eraseSufixComments(rawLine));
		raw.erase(0, raw.find('\n') + 1);
		if (line.size() == 0) // skip empty line
			continue;
		if (line.find("server") != std::string::npos && ParserUtils::eraseOWS(line).compare("server{") == 0)
		{
			_servers.push_back(Server());
			_servers.back().readInfos(raw); // no try catch needed here i guess would be caught higher
			continue;
		}
		else
			throw std::invalid_argument("Bad config line: " + rawLine);
	}
}

//	Config specific getters
std::string const & ConfigFile::getRawData() const {return (_rawData);}
std::vector<Server> const & ConfigFile::getServers() const {return (_servers);}

//	Getters for children
std::string const & ConfigFile::getRoot() const {return _root;}
std::string const & ConfigFile::getIndex() const {return _index;}
std::string const & ConfigFile::getAutoIndex() const {return _autoindex;}
std::string const & ConfigFile::getMaxBodySize() const {return _maxBodysize;}
std::string const & ConfigFile::getRedirection() const {return _redirection;}
std::map<std::string, std::string> const & ConfigFile::getErrorPages() const {return _errorPages;}
std::vector<std::string> const & ConfigFile::getAllowMethods() const {return _allowMethods;}
std::vector<std::string> const & ConfigFile::getCgiPathes() const {return _cgiPathes;}
std::vector<std::string> const & ConfigFile::getCgiExt() const {return _cgiExt;}

//	Setters for children
void	ConfigFile::setRoot(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws): " + line);
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	_root = element;
}

void	ConfigFile::setIndex(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws):" + line);
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	_index = element;
}

void	ConfigFile::setAutoIndex(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (element.compare("on") && element.compare("off"))
		throw std::invalid_argument("Bad config line (autoindex == on or off)");
	_autoindex = element;
}

void	ConfigFile::setMaxBodySize(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws):" + line);
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	if (ParserUtils::isStrDigit(element) == false)
		throw std::invalid_argument("Bad config line (maxbodysize not digit):" + line);

	std::stringstream	str(element);
	unsigned int		i;
	str >> i;
	if (str.fail() || !str.eof())
		throw std::invalid_argument("Bad config line (maxbodysize vs unsigned int):" + line);
	_maxBodysize = element;
}

void	ConfigFile::setErrorPages(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1))); // "404 /404.html"
	if ((ows_pos = ParserUtils::firstWsPos(element)) == -1)
		throw std::invalid_argument("Bad config line (err pages needs 2 elements: code and path)");

	std::string	key(ParserUtils::trimOWS(element.substr(0, ows_pos)));					// "404"
	std::string	val(ParserUtils::trimOWS(element.substr(ows_pos, element.size() -1)));	// "/404.html"

	if (ParserUtils::firstWsPos(val) != -1 || val.size() == 0 )
		throw std::invalid_argument("Bad config line (extra ws or empty page path):" + line);
	if (key.size() != 3 || ParserUtils::isStrDigit(key) == false)
		throw std::invalid_argument("Bad config line (bad error code size or not digit): " + line);
	_errorPages[key] = val;
}

void	ConfigFile::setAllowMethods(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1))); // "GET, POST, DELETE"
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	while (element.size() > 0)
	{
		unsigned int	sep_pos = element.find(',') != std::string::npos ? element.find(',') : element.size();
		std::string	method(ParserUtils::trimOWS(element.substr(0, sep_pos)));
		if (method.compare("GET") && method.compare("POST") && method.compare("DELETE"))
			throw std::invalid_argument("Bad config line (only methods GET,POST,DELETE) :" + line);
		element.erase(0, sep_pos +  1);
		_allowMethods.push_back(method);
	}
}

void	ConfigFile::setCgiPathes(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	while (element.size() > 0)
	{
		unsigned int	sep_pos = element.find(',') != std::string::npos ? element.find(',') : element.size();
		std::string	path(ParserUtils::trimOWS(element.substr(0, sep_pos)));
		element.erase(0, sep_pos +  1);
		if (path.size() == 0)
			throw std::invalid_argument("Bad config line (empty val): " + line);
		_cgiPathes.push_back(path);
	}
}

void	ConfigFile::setCgiExt(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	while (element.size() > 0)
	{
		unsigned int	sep_pos = element.find(',') != std::string::npos ? element.find(',') : element.size();
		std::string	ext(ParserUtils::trimOWS(element.substr(0, sep_pos)));
		element.erase(0, sep_pos +  1);
		if (ext.size() == 0)
			throw std::invalid_argument("Bad config line (empty val): " + line);
		_cgiExt.push_back(ext);
	}
}
