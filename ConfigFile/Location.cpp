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
		_isPathAbsolute = rhs.getIsPathAbsolute();
		_root = rhs.getRoot();
		_index = rhs.getIndex();
		_autoindex = rhs.getAutoIndex();
		_maxBodysize = rhs.getMaxBodySize();
		_allowMethods = rhs.getAllowMethods();
		_redirection = rhs.getRedirection();
		_errorPages = rhs.getErrorPages();
		_cgiPathes = rhs.getCgiPathes();
		_cgiExt = rhs.getCgiExt();
		_allowMethods = rhs.getAllowMethods();
	}
	return (*this);
}

//	printer
void	Location::printAll() const
{
	std::cout << "#### Location printer ####" << std::endl;
	std::cout << "Path:\t" + getPath() << std::endl;
	std::cout << "Absolute path?:\t" << getIsPathAbsolute() << std::endl;
	std::cout << "Root:\t" + getRoot() << std::endl;
	std::cout << "Index:\t" + getIndex() << std::endl;
	std::cout << "Autoindex:\t" + getAutoIndex() << std::endl;
	std::cout << "Maxbodysize:\t" + getMaxBodySize() << std::endl;
	std::cout << "Redirection:\t" + getRedirection() << std::endl;
	for (std::map<std::string,std::string>::const_iterator it = getErrorPages().begin(); it != getErrorPages().end(); ++it)
		std::cout << "errPages:\t" + it->first + "-->" + it->second << std::endl;
	for (std::vector<std::string>::const_iterator it = getAllowMethods().begin(); it != getAllowMethods().end(); ++it)
		std::cout << "allow method:\t" + *it << std::endl;
	for (std::vector<std::string>::const_iterator it = getCgiExt().begin(); it != getCgiExt().end(); ++it)
		std::cout << "cgi ext:\t" + *it << std::endl;
	for (std::vector<std::string>::const_iterator it = getCgiPathes().begin(); it != getCgiPathes().end(); ++it)
		std::cout << "cgi path:\t" + *it << std::endl;
	std::cout << "#### end of location ####" << std::endl;
}

//	getters
std::string const & Location::getPath() const {return _path;}
bool const & Location::getIsPathAbsolute() const {return _isPathAbsolute;}

//	special location setters
void	Location::setPath(std::string ogline)	// actually called in server part
{
	std::string	line(ogline);
	line.erase(0, line.find("location") + 8);
	line.erase(line.find('{'), 1);
	line = ParserUtils::trimOWS(line);

	_isPathAbsolute = false;
	if (line[0] == '=' && isspace(line[1]))
	{
		_isPathAbsolute = true;
		line.erase(0, 1);
		line = ParserUtils::trimOWS(line);
	}
	if (ParserUtils::firstWsPos(line) != -1)
		throw std::invalid_argument("No whitespace allowed in location path: " + ogline);
	if (line.size() == 0)
		throw std::invalid_argument("Bad config line (empty location path): " + ogline);
	_path = line;
}

void	Location::setRedirection(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);
	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws):" + line);
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	_redirection = element;
}

/*
	reads all location info
*/
void	Location::readInfos(std::string & raw)
{
	std::string	line, rawLine;

	while (raw.find('\n') != std::string::npos)
	{
		rawLine = raw.substr(0, raw.find('\n'));
		line = ParserUtils::trimOWS(ParserUtils::eraseSufixComments(rawLine));
		raw.erase(0, raw.find('\n') + 1);
		if (line.size() == 0)
			continue;
		if (line.compare("}") == 0)
			break;
		else
		{
			if (ParserUtils::checkRmSemiColon(line) == false)
				throw std::invalid_argument("Bad config line (no terminating semicolon): " + rawLine);
			if (isValidElementLabel(line) == false)
				throw std::invalid_argument("Bad config element found: " + rawLine);
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
	void (Location::*ptrFct[9])(std::string) = { &ConfigFile::setRoot, &ConfigFile::setIndex,
								&ConfigFile::setAutoIndex, &ConfigFile::setMaxBodySize,
								&Location::setRedirection, &ConfigFile::setErrorPages,
								&ConfigFile::setAllowMethods, &ConfigFile::setCgiPathes,
								&ConfigFile::setCgiExt};

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
