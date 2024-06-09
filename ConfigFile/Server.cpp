#include"Server.hpp"

//	Default
Server::Server() {} 
Server::~Server() {}
//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const Server &rhs)
{
	return (os << "Server " << rhs.getName() << " listens on " << rhs.getIp() << ":" << rhs.getPort());
}
//	Param constructor
Server::Server(const std::string name)
{
	_name = name;
}

//	Copy
Server::Server(const Server & src) {*this = src;}
Server & Server::operator=(const Server & rhs)
{
	if(this != &rhs)
	{
		_name = rhs.getName();
		_ip = rhs.getIp();
		_port = rhs.getPort();
		_root = rhs.getRoot();
		_autoindex = rhs.getAutoindex();
		_index = rhs.getIndex();
	}
	return (*this);
}

//	Getters
std::string const & Server::getName() const	{return (_name);}
std::string const & Server::getIp() const	{return (_ip);}
std::string const & Server::getPort() const	{return (_port);}
std::string const & Server::getRoot() const	{return (_root);}
std::string const & Server::getAutoindex() const	{return (_autoindex);}
std::string const & Server::getIndex() const	{return (_index);}



