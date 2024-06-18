#include"Server.hpp"

//	constructors + copy
Server::Server() {} 
Server::~Server() {}
Server::Server(const Server & src) {*this = src;}
Server & Server::operator=(const Server & rhs)
{
	if(this != &rhs)
	{
		_names = rhs.getNames();
		_ip = rhs.getIp();
		_port = rhs.getPort();
		_root = rhs.getRoot();
		_index = rhs.getIndex();
		_autoindex = rhs.getAutoIndex();
		_maxBodysize = rhs.getMaxBodySize();
		_errorPages = rhs.getErrorPages();
		_allowMethods = rhs.getAllowMethods();
		_cgiPathes = rhs.getCgiExt();
		_cgiExt = rhs.getCgiPathes();
		_locations = rhs.getLocations();
	}
	return (*this);
}

//	printer
void	Server::printAll() const
{
	std::cout << "#### Server printer ####" << std::endl;
	std::cout << "Ip:\t" + getIp() << std::endl;
	std::cout << "Port:\t" << getPort() << std::endl;
	for (std::vector<std::string>::const_iterator it = getNames().begin(); it != getNames().end(); ++it)
		std::cout << "servername:\t" + *it << std::endl;
	std::cout << "Root:\t" + getRoot() << std::endl;
	std::cout << "Index:\t" + getIndex() << std::endl;
	std::cout << "Autoindex:\t" + getAutoIndex() << std::endl;
	std::cout << "Maxbodysize:\t" + getMaxBodySize() << std::endl;
	for (std::map<std::string,std::string>::const_iterator it = getErrorPages().begin(); it != getErrorPages().end(); ++it)
		std::cout << "errPages:\t" + it->first + "-->" + it->second << std::endl;
	for (std::vector<std::string>::const_iterator it = getAllowMethods().begin(); it != getAllowMethods().end(); ++it)
		std::cout << "allow method:\t" + *it << std::endl;
	for (std::vector<std::string>::const_iterator it = getCgiExt().begin(); it != getCgiExt().end(); ++it)
		std::cout << "cgi ext:\t" + *it << std::endl;
	for (std::vector<std::string>::const_iterator it = getCgiPathes().begin(); it != getCgiPathes().end(); ++it)
		std::cout << "cgi path:\t" + *it << std::endl;
	for (std::vector<Location>::const_iterator it = getLocations().begin(); it != getLocations().end(); ++it)
		it->printAll();
	std::cout << "#### end of server ####" << std::endl;
}

//	getters
std::vector<std::string> const & Server::getNames() const	{return (_names);}
std::string const & Server::getIp() const	{return (_ip);}
std::string const & Server::getPort() const	{return (_port);}
std::vector<Location> const & Server::getLocations() const {return (_locations);}

//	setters
void	Server::setNames(std::string line)
{

}

void	Server::setIpPort(std::string line)
{

}

/*
	reads all server info, looks for locations
*/
void	Server::readInfos(std::string & raw)
{
	std::string	line, rawLine;

	while (raw.find('\n') != std::string::npos)
	{
		rawLine = raw.substr(0, raw.find('\n'));
		line = ParserUtils::trimOWS(ParserUtils::eraseSufixComments(rawLine));
		raw.erase(0, raw.find('\n') + 1);
		if (line.size() == 0) // skip empty line
			continue;
		if (line.find("location") != std::string::npos && line.find('{') != std::string::npos)
		{
			_locations.push_back(Location());
			_locations.back().setPath(line);
			_locations.back().readInfos(raw);
		}
		else if (line.compare("}") == 0) // get back to config part
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
bool	Server::isValidElementLabel(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (label) (no ws):" + line);
	std::string	label = line.substr(0, ows_pos);
	std::string valid[2] = {"server_names", "listen",}; // "root", "index", "autoindex","max_body_size", "error_page", "allow_methods", "cgi", "cgi_ext"};
	void (Server::*ptrFct[2])(std::string) = { &Server::setNames, &Server::setIpPort,}; //&Location::setIndex,&Location::setAutoIndex, &Location::setMaxBodySize,&Location::setRedirection, &Location::setErrorPages,&Location::setAllowMethods, &Location::setCgiPathes,&Location::setCgiExt};

	for (int i = 0; i < 10; ++i)
	{
		if (label.compare(valid[i]) == 0)
		{
			(this->*ptrFct[i])(line);
			return (true);
		}
	}
	return false;
}
