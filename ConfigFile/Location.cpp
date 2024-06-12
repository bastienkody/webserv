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
		_maxBodysize = rhs.getMaxBodySize();
		_allowMethods = rhs.getAllowMethods();
		_redirection = rhs.getRedirection();
		_errorPages = rhs.getErrorPages();
		_cgiPathes = rhs.getCgiExt();
		_cgiExt = rhs.getCgiExt();
		_allowMethods = rhs.getAllowMethods();
	}
	return (*this);
}

//	getters
std::string const & Location::getPath() const {return _path;}
std::string const & Location::getRoot() const {return _root;}
std::string const & Location::getIndex() const {return _index;}
std::string const & Location::getAutoIndex() const {return _autoindex;}
std::string const & Location::getMaxBodySize() const {return _maxBodysize;}
std::string const & Location::getAllowMethods() const {return _allowMethods;}
std::map<std::string, std::string> const & Location::getErrorPages() const {return _errorPages;}
std::vector<std::string> const & Location::getCgiPathes() const {return _cgiPathes;}
std::vector<std::string> const & Location::getCgiExt() const {return _cgiExt;}

//	setters
void	Location::setPath(std::string ogline)	// actually called in server part
{
	std::string	line(ogline);
	line.erase(0, line.size() -1);		// erase "location"
	line.erase(line.find('{'), 1);		// erase '{'
	line = ParserUtils::trimOWS(line);	// get "= path" or "path"

	_isPathAbsolute = false;
	if (line[0] == '=' && isspace(line[1]))	// absolute matching path?
	{
		_isPathAbsolute = true;
		line.erase(0, 1);
		line = ParserUtils::trimOWS(line);	// get "path"
	}

	for (std::string::iterator it = line.begin(); it != line.end(); ++it) // check for remaining spaces in  path
		if (isspace(*it))
			throw std::invalid_argument("No space allowed in location path: " + ogline);
}

void	Location::setRoot()
{

}



bool	Location::isValidElementLabel(std::string line)
{
	std::string valid[] = {"root", "index", "autoindex", "max_body_size",
							"allow_methods", "redirection", "error_page", 
							"cgi", "cgi_ext",
							};
	void (*ptrFct[3])(std::string) = { setRoot, setIndex, setAutoIndex,
							 };
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
		if (line.size() == 0) // skip empty line
			continue;
		std::cout << line;
		if (line.compare("}") == 0) // get back to config part
			break;
		else
		{
			std::cout << "\tlocation attribute" << std::endl;
			if (isValidElementLabel(line) == false)
				throw std::invalid_argument("Bad config element found: " + line);
		}
	}
}
