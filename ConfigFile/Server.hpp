#ifndef SERVER_HPP
#define SERVER_HPP

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "Location.hpp"

class Location;

class Server
{
	public:
		Server();
		~Server();
		//Server(const std::string name);
		Server(const Server & src);
		Server & operator=(const Server & rhs);

		std::vector<std::string> const & getNames() const;
		std::string const & getIp() const;
		std::string const & getPort() const;
		std::string const & getRoot() const;
		std::string const & getIndex() const;
		std::string const & getAutoindex() const;
		std::string const & getMaxBodySize() const;
		std::string const & getAllowMethods() const;
		std::map<std::string,std::string> const & getErrorPages() const;
		std::vector<std::string> const & getCgiPathes() const;
		std::vector<std::string> const & getCgiExt() const;

		void	readInfos(std::string & raw); //throw

	private:
		std::vector<std::string> _names;
		std::string _ip;
		std::string _port;
		std::string _root;
		std::string	_index;
		std::string	_autoindex;
		std::string	_maxBodysize;	//stored as a string but ovlf checks needed when reading + use of neg (not zero) if not provided
		std::string	_allowMethods;
		std::map<std::string,std::string> _errorPages;
		std::vector<std::string> _cgiPathes;
		std::vector<std::string> _cgiExt;
		std::vector<Location> _locations;
};

#endif
