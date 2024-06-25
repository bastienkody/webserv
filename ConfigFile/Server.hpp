#ifndef SERVER_HPP
#define SERVER_HPP

#include<iostream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<vector>
#include<map>
#include "ConfigFile.hpp"
#include "Location.hpp"

#define MAX_PORTS_NUMBER 65635

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
		unsigned int const & getPort() const;
		std::string const & getPortSTR() const;
		std::vector<Location> const & getLocations() const;
	// setters
		void	setNames(std::string line);
		void	setIpPort(std::string line);

		bool	checkIpv4(std::string line);
		bool	checkPort(std::string line);

		void	readInfos(std::string & raw); //throw
		bool	isValidElementLabel(std::string line);

	private:
		std::vector<std::string>	_names;
		std::string					_ip;
		std::string					_portSTR;
		unsigned int				_port;
		std::vector<Location>		_locations;
};

#endif
