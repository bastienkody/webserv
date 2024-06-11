#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "Server.hpp"
#include "../ParserUtils/ParserUtils.hpp"

class Server;

class ConfigFile
{
	public:
		ConfigFile();
		~ConfigFile();
		ConfigFile(const std::string filename);
		ConfigFile(const ConfigFile & src);
		ConfigFile & operator=(const ConfigFile & rhs);

		std::string const & getRawData() const;
		std::vector<Server> const & getServers() const;

		void	openReadFileToStr(); // throw
		void	readAllInfos(); // throw

	private:
		std::fstream	_fs;
		std::string		_filename;
		std::string		_rawData;

		std::vector<Server>	_servers;

};

std::ostream& operator<<(std::ostream& os, const ConfigFile &rhs);

#endif
