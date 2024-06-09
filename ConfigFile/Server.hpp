#ifndef SERVER_HPP
#define SERVER_HPP

#include<iostream>
#include<string>
#include<vector>

class Server
{
	public:
		Server();
		~Server();
		Server(const std::string name);
		Server(const Server & src);
		Server & operator=(const Server & rhs);

		std::string const & getName() const;
		std::string const & getIp() const;
		std::string const & getPort() const;
		std::string const & getRoot() const;
		std::string const & getAutoindex() const;
		std::string const & getIndex() const;

	private:
		std::string _name;

		std::string _ip;
		std::string _port;

		std::string _root;

		std::string	_autoindex;
		std::string	_index;




};

std::ostream& operator<<(std::ostream& os, const Server &rhs);

#endif
