#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "Server.hpp"

/*	
	Opens config file outside of the constructor to throw and still have the object
*/

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

		void	openReadFileData();

	private:
		std::fstream	_fs;
		std::string		_filename;
		std::string		_rawData;

		std::vector<Server>	_servers;

};

std::ostream& operator<<(std::ostream& os, const ConfigFile &rhs);

#endif
