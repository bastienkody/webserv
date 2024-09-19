#include"Server.hpp"
#include"Location.hpp"

//	constructors + copy
Server::Server(): _ip("0.0.0.0"), _port(8080), _portSTR("8080")	{}
Server::~Server()
{
	_names.clear();
	_ip.clear();
	_portSTR.clear();
	_locations.clear();
}
Server::Server(const Server & src): ConfigFile() {*this = src;}
Server & Server::operator=(const Server & rhs)
{
	if(this != &rhs)
	{
		_names = rhs.getNames();
		_ip = rhs.getIp();
		_port = rhs.getPort();
		_portSTR = rhs.getPortSTR();
		_root = rhs.getRoot();
		_index = rhs.getIndex();
		_autoindex = rhs.getAutoIndex();
		_maxBodysize = rhs.getMaxBodySize();
		_redirection = rhs.getRedirection();
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
	std::cout << "#### Server printer (location nb:" << getLocations().size() << ") ####" << std::endl;
	std::cout << "Ip:\t" + getIp() << std::endl;
	std::cout << "Port:\t" << getPort() << std::endl;
	std::cout << "PortSTR:\t" << getPortSTR() << std::endl;
	for (std::vector<std::string>::const_iterator it = getNames().begin(); it != getNames().end(); ++it)
		std::cout << "name:\t" + *it << std::endl;
	std::cout << "Root:\t" + getRoot() << std::endl;
	std::cout << "Index:\t" + getIndex() << std::endl;
	std::cout << "Autoindex:\t" + getAutoIndex() << std::endl;
	std::cout << "Maxbodysize:\t" + getMaxBodySize() << std::endl;
	for (std::map<std::string,struct rewrite>::const_iterator it = getRedirection().begin(); it != getRedirection().end(); ++it)
		std::cout << "redir:\t" + it->first + "-->" + it->second.redirect_url + " type:" << it->second.type << std::endl;
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
unsigned int const & Server::getPort() const	{return (_port);}
std::string const & Server::getPortSTR() const	{return (_portSTR);}
std::vector<Location> const & Server::getLocations() const {return (_locations);}

//	setters
void	Server::setNames(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);
	while (element.size() > 0)
	{
		ows_pos = ParserUtils::firstWsPos(element);
		if (ows_pos == -1)
			ows_pos = element.size();
		std::string	name(ParserUtils::trimOWS(element.substr(0, ows_pos)));
		element.erase(0, ows_pos +  1);
		_names.push_back(name);
	}
}

bool	Server::checkIpv4(std::string line)
{
	unsigned int		sep_pos;

	if (ParserUtils::charCount(line, '.') != 3)
		return false;

	for (int i = 0; i < 4; ++i)
	{
		sep_pos = (i != 3 ? line.find('.') : line.size());
		std::stringstream	sstr(line.substr(0, sep_pos));
		line.erase(0, sep_pos + 1);
		if (sstr.str().size() > 3 || ParserUtils::isStrDigit(sstr.str()) == false)
			return  false;
		unsigned int	nb;
		sstr >> nb;
		if (sstr.fail() || !sstr.eof() || nb > 255)
			return false;
	}
	return true;
}

bool	Server::checkPort(std::string line)
{
	std::stringstream	sstr(line);
	unsigned int		port;

	sstr >> port;
	if (sstr.fail() || !sstr.eof() || port >= MAX_PORTS_NUMBER || ParserUtils::isStrDigit(line) == false)
		return false;
	return true;
}

void	Server::setIpPort(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (no ws): " + line);

	std::string	element(ParserUtils::trimOWS(line.substr(ows_pos + 1, line.size() -1)));
	if (ParserUtils::firstWsPos(element) != -1)
		throw std::invalid_argument("Bad config line (extra ws): " + line);
	if (element.size() == 0)
		throw std::invalid_argument("Bad config line (empty val): " + line);

	if (element.find(':') != std::string::npos)
	{
		std::string	ip = element.substr(0, element.find(':'));
		if (checkIpv4(ip) == false && ip != "localhost")
			throw std::invalid_argument("Bad config line (invalid IP address): " + line);
		_ip = (ip == "localhost" ? "127.0.0.1" : ip);
		std::string	port = element.substr(element.find(':') + 1, element.size() - 1);
		if (checkPort(port) == false)
			throw std::invalid_argument("Bad config line (invalid port): " + line);
		// std::cerr<< "port s: " << port<<std::endl;
		_portSTR = port;
		// std::cerr<< "port s2: " << _portSTR<<std::endl;
		_port = std::atoi(port.c_str());
		return;
	}
	if (element.find('.') != std::string::npos)
	{
		if (checkIpv4(element) == false)
			throw std::invalid_argument("Bad config line (invalid IP address): " + line);
		_ip = element;
	}
	else
	{
		if (checkPort(element) == false)
			throw std::invalid_argument("Bad config line (invalid port): " + line);
		_portSTR = element;
		_port = std::atoi(element.c_str());
	}
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
			// std::cerr<<" portSTR 2: " << _portSTR<<std::endl;
		}
	}
	if (line.compare("}") != 0)
		throw std::invalid_argument("Bad config: server is not closed (expecting closing bracket)");

}

// must add a check on redefinition of element (ie. 2 index) --> nginx tests needed
bool	Server::isValidElementLabel(std::string line)
{
	int	ows_pos = ParserUtils::firstWsPos(line);
	if (ows_pos == -1)
		throw std::invalid_argument("Bad config line (label) (no ws):" + line);
	std::string	label = line.substr(0, ows_pos);
	std::string valid[11] = {"server_name", "listen", "root", "rewrite", "index", "autoindex","max_body_size", "error_page", "allow_methods", "cgi", "cgi_ext"};
	void (Server::*ptrFct[11])(std::string) = { &Server::setNames, &Server::setIpPort,
												&ConfigFile::setRoot, &ConfigFile::setRedirections, &ConfigFile::setIndex,
												&ConfigFile::setAutoIndex, &ConfigFile::setMaxBodySize,
												&ConfigFile::setErrorPages, &ConfigFile::setAllowMethods,
												&ConfigFile::setCgiPathes, &ConfigFile::setCgiExt};

	for (int i = 0; i < 11; ++i)
	{
		if (label.compare(valid[i]) == 0)
		{
			(this->*ptrFct[i])(line);
			// std::cerr << i << " portSTR : " << _portSTR<<std::endl;
			return (true);
		}
	}
	return false;
}
