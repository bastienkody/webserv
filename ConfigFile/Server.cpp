#include"Server.hpp"

//	Default
Server::Server() {} 
Server::~Server() {}
//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const Server &rhs)
{
	return (os << "Server listens on " << rhs.getIp() << ":" << rhs.getPort());
}
//	Param constructor
Server::Server(const std::string data) {}

//	Copy
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

//	Getters
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



