#include"Location.hpp"

// constructors + copy
Location::Location() {}
Location::~Location() {}
Location::Location(const Location & src) {*this = src;}
Location & Location::operator=(const Location & rhs)
{
	if(this != &rhs)
	{
		_path = rhs.getPath();
		_root = rhs.getRoot();
		_index = rhs.getIndex();
		_autoindex = rhs.getAutoIndex();
		/*_maxBodysize = rhs.getMaxBodySize();
		_allowMethods = rhs.getAllowMethods();
		_redirection = rhs.getRedirection();
		_errorPages = rhs.getErrorPages();
		_cgiPathes = rhs.getCgiExt();
		_cgiExt = rhs.getCgiExt();
		_allowMethods = rhs.getAllowMethods();*/
	}
	return (*this);
}

//	getters
std::string const & Location::getPath() const {return _path;}
bool const & Location::getIsPathAbsolute() const {return _isPathAbsolute;}
std::string const & Location::getRoot() const {return _root;}
std::string const & Location::getIndex() const {return _index;}
std::string const & Location::getAutoIndex() const {return _autoindex;}
std::string const & Location::getMaxBodySize() const {return _maxBodysize;}
std::string const & Location::getRedirection() const {return _redirection;}
std::map<std::string, std::string> const & Location::getErrorPages() const {return _errorPages;}
std::vector<std::string> const & Location::getAllowMethods() const {return _allowMethods;}
std::vector<std::string> const & Location::getCgiPathes() const {return _cgiPathes;}
std::vector<std::string> const & Location::getCgiExt() const {return _cgiExt;}

//	setters
void	Location::setPath(std::string ogline)	// actually called in server part
{
	std::string	line(ogline);
	line.erase(0, line.find("location") + 8);
	line.erase(line.find('{'), 1);
	line = ParserUtils::trimOWS(line);
	std::cout << "\npath after parse:" + line << std::endl;

	_isPathAbsolute = false;
	if (line[0] == '=' && isspace(line[1]))
	{
		_isPathAbsolute = true;
		line.erase(0, 1);
		line = ParserUtils::trimOWS(line);
	}
	if (ParserUtils::firstWsPos(line) != -1)
		throw std::invalid_argument("No whitespace allowed in location path: " + ogline);
	_path = line;
}

void	Location::setRoot(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws): " + line);
	_root = element;
}

void	Location::setIndex(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws):" + line);
	_index = element;
}

void	Location::setAutoIndex(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (element.compare("on") && element.compare("off"))
		throw std::invalid_argument("Bad config line (autoindex == on or off)");
	_autoindex = element;
}

void	Location::setMaxBodySize(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws):" + line);
	for (std::string::iterator it = element.begin(); it != element.end(); ++it)
		if (isdigit(*it) == false)
			throw std::invalid_argument("Bad config line (maxbodysize not digit):" + line);

	std::stringstream	str(element);
	unsigned int		i;
	str >> i;
	if (str.fail() || !str.eof())
		throw std::invalid_argument("Bad config line (maxbodysize vs unsigned int):" + line);
	_maxBodysize = element;
}

void	Location::setRedirection(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws):" + line);
	_redirection = element;
}

void	Location::setErrorPages(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1))); // "404 /404.html"
	if ((ows_pos = ParserUtils::firstWsPos(element)) == -1)
		throw std::invalid_argument("Bad config line (err pages needs 2 elements: code and path)");

	std::string	key(ParserUtils::trimOWS(element.substr(0, ows_pos)));					// "404"
	std::string	val(ParserUtils::trimOWS(element.substr(ows_pos, element.size() -1)));	// "/404.html"

	if (ParserUtils::firstWsPos(val) != -1)
		throw std::invalid_argument("Bad config line (extra ws in error page path):" + line);
	_errorPages[key] = val;
}

void	Location::setAllowMethods(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1))); // "GET, POST, DELETE"
	while (element.size() > 0)
	{
		unsigned int	sep_pos = element.find(',') != std::string::npos ? element.find(',') : element.size();
		std::string	method(ParserUtils::trimOWS(element.substr(0, sep_pos)));
		if (method.compare("GET") && method.compare("POST") && method.compare("DELETE"))
			throw std::invalid_argument("Bad method found on allow_method line:" + line);
		element.erase(0, sep_pos +  1);
		_allowMethods.push_back(method);
	}
}

void	Location::setCgiPathes(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	while (element.size() > 0)
	{
		unsigned int	sep_pos = element.find(',') != std::string::npos ? element.find(',') : element.size();
		std::string	path(ParserUtils::trimOWS(element.substr(0, sep_pos)));
		element.erase(0, sep_pos +  1);
		_cgiPathes.push_back(path);
	}
}

void	Location::setCgiExt(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1))); // "GET, POST, DELETE"
	while (element.size() > 0)
	{
		unsigned int	sep_pos = element.find(',') != std::string::npos ? element.find(',') : element.size();
		std::string	ext(ParserUtils::trimOWS(element.substr(0, sep_pos)));
		element.erase(0, sep_pos +  1);
		_cgiExt.push_back(ext);
	}
}

/*
	reads all location info 
*/
void	Location::readInfos(std::string & raw)
{
	std::cout << "from location.readinfo() :" << std::endl;
	std::string	line, rawLine;

	while (raw.find('\n') != std::string::npos)
	{
		rawLine = raw.substr(0, raw.find('\n'));
		line = ParserUtils::trimOWS(ParserUtils::eraseSufixComments(rawLine));
		raw.erase(0, raw.find('\n') + 1);
		if (line.size() == 0)
			continue;
		std::cout << line;
		if (line.compare("}") == 0)
			break;
		else
		{
			if (ParserUtils::checkRmSemiColon(line) == false)
				throw std::invalid_argument("Bad config line (no terminating semicolon): " + line);
			std::cout << "\tlocation attribute" << std::endl;
			if (isValidElementLabel(line) == false)
				//throw std::invalid_argument("Bad config element found: " + line);
				std::cout << "bad line for now" << std::endl;
		}
	}
}

// must add a check on redefinition of element (ie. 2 index) --> nginx tests needed
bool	Location::isValidElementLabel(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (label) (no ws):" + line);
	std::string	label = line.substr(0, ows_pos);
	std::string valid[9] = {"root", "index", "autoindex", "max_body_size", "redirection",
							"error_page", "allow_methods", "cgi", "cgi_ext"};
	void (Location::*ptrFct[9])(std::string) = { &Location::setRoot, &Location::setIndex,
								&Location::setAutoIndex, &Location::setMaxBodySize,
								&Location::setRedirection, &Location::setErrorPages,
								&Location::setAllowMethods, &Location::setCgiPathes,
								&Location::setCgiExt};

	std::cout << "line:" + line + "&&label:" + label<< std::endl;
	for (int i = 0; i < 9; ++i)
	{
		if (label.compare(valid[i]) == 0)
		{
			(this->*ptrFct[i])(line);
			return (true);
		}
	}
	return false;
}
