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
		_autoindex = rhs.getAutoindex();
		_maxBodysize = rhs.getMaxBodySize();
		_allowMethods = rhs.getAllowMethods();
		_errorPages = rhs.getErrorPages();
		_cgiPathes = rhs.getCgiExt();
		_cgiExt = rhs.getCgiExt();
		_allowMethods = rhs.getAllowMethods();
	}
	return (*this);
}

//	getters
std::vector<std::string> const & Server::getNames() const	{return (_names);}
std::string const & Server::getIp() const	{return (_ip);}
std::string const & Server::getPort() const	{return (_port);}
std::string const & Server::getRoot() const	{return (_root);}
std::string const & Server::getIndex() const	{return (_index);}
std::string const & Server::getAutoindex() const	{return (_autoindex);}
std::string const & Server::getMaxBodySize() const	{return (_maxBodysize);}
std::string const & Server::getAllowMethods() const	{return (_allowMethods);}
std::map<std::string,std::string> const & Server::getErrorPages() const {return (_errorPages);}
std::vector<std::string>const & Server::getCgiPathes() const	{return (_cgiPathes);}
std::vector<std::string> const & Server::getCgiExt() const	{return (_cgiExt);}

//	setters

/*
	reads all server info, looks for locations
*/
void	Server::readInfos(std::string & raw)
{
	std::cout << "from server.readinfo() :" << std::endl;
	std::string	line, rawLine;

	while (raw.find('\n') != std::string::npos)
	{
		rawLine = raw.substr(0, raw.find('\n'));
		line = ParserUtils::trimOWS(ParserUtils::eraseSufixComments(rawLine));
		raw.erase(0, raw.find('\n') + 1);
		if (line.size() == 0) // skip empty line
			continue;
		std::cout << line;
		if (line.find("location") != std::string::npos) // && ParserUtils::eraseOWS(line).compare("server{") == 0) // to be modified to get the path
		{
			_locations.push_back(Location());
			_locations.back().setPath(line);
			_locations.back().readInfos(raw);
		}
		else if (line.compare("}") == 0) // get back to config part
			break;
		else
			std::cout << "\tserver attribute" << std::endl;
	}

}
