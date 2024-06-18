#ifndef SERVER_HPP
#define SERVER_HPP

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "ConfigFile.hpp"
#include "Location.hpp"

class ConfigFile;

class Server: public ConfigFile
{
	public:
		Server();
		~Server();
		Server(const Server & src);
		Server & operator=(const Server & rhs);
	// printer
		void	printAll() const;
	// getters
		std::vector<std::string> const & getNames() const;
		std::string const & getIp() const;
		std::string const & getPort() const;
		std::vector<Location> const & getLocations() const;
	// setters
		void	setNames(std::string line);
		void	setIpPort(std::string line);

		void	readInfos(std::string & raw); //throw
		bool	isValidElementLabel(std::string line);

	private:
		std::vector<std::string>	_names;
		std::string					_ip;
		std::string					_port;
		std::vector<Location>		_locations;
};

#endif
