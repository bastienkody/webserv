#include"ConfigFile.hpp"

//	Default
ConfigFile::ConfigFile() {}
ConfigFile::~ConfigFile() {}
//	Param constructor
ConfigFile::ConfigFile(const std::string filename) : _filename(filename) {}
//	Copy constructor (+ copy operator)
ConfigFile::ConfigFile(const ConfigFile & src) {*this = src;}
ConfigFile & ConfigFile::operator=(const ConfigFile & rhs)
{
	if(this != &rhs)
	{
		this->_filename = rhs._filename;
		this->_rawData = rhs.getRawData();
	}
	return (*this);
}
//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const ConfigFile &rhs)
{return (os << rhs.getRawData());} 
//	Getter
std::string const & ConfigFile::getRawData() const {return (_rawData);}
std::vector<Server> const & ConfigFile::getServers() const {return (_servers);}

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

/*
	read the whole config file line by line (erase comment + trim OWS) looking for server parts
*/
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
		std::cout << line <<std::endl;
		std::cout << line.find("server") <<std::endl;
		std::cout << ParserUtils::eraseOWS(line).compare("server{") <<std::endl;

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




