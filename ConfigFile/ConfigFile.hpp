#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include "../ParserUtils/ParserUtils.hpp"

struct rewrite{
	std::string	redirect_url;
	int			type; // 301 or 302
};

class Server;

class ConfigFile
{
	public:
		ConfigFile();
		~ConfigFile();
		ConfigFile(const std::string filename);
		ConfigFile(const ConfigFile & src);
		ConfigFile & operator=(const ConfigFile & rhs);
	//	printer
		void	printAll() const;
	//	getters
		std::string const & getRawData() const;
		std::vector<Server> const & getServers() const;
		int getServerFromFd(int fd);
	// setter
		void	setServerFd(int fd, int server_nb);
	//	getters for children
		std::string const & getRoot() const;
		std::string const & getIndex() const;
		std::string const & getAutoIndex() const;
		std::string const & getMaxBodySize() const;
		std::map<std::string, struct rewrite> const & getRedirection() const;
		std::map<std::string, std::string> const & getErrorPages() const;
		std::vector<std::string> const & getAllowMethods() const;
		std::vector<std::string> const & getCgiExt() const;
		std::vector<std::string> const & getCgiPathes() const;
	//	setters for children
		void	setRoot(std::string line);
		void	setIndex(std::string line);
		void	setAutoIndex(std::string line);
		void	setMaxBodySize(std::string line);
		void	setAllowMethods(std::string line);
		void	setRedirections(std::string line);
		void	setErrorPages(std::string line);
		void	setCgiPathes(std::string line);
		void	setCgiExt(std::string line);

		void	openReadFileToStr(); // throw
		void	readAllInfos(); // throw

	private:
		std::fstream		_fs;
		std::string			_filename;
		std::string			_rawData;
		std::vector<Server>	_servers;
		std::map<int, int>	_fd_to_server_nb;
	protected:
		std::string	_root;
		std::string	_index;
		std::string	_autoindex;
		std::string	_maxBodysize;	//stored as a string but ovlf checks needed when reading + use of neg (not zero) if not provided
		// upload_files ?? i dont understand how nginx deals with it ; may we do smthg different?
		std::map<std::string, struct rewrite>	_redirection;
		std::map<std::string,std::string>	_errorPages;
		std::vector<std::string>	_allowMethods;
		std::vector<std::string>	_cgiExt;
		std::vector<std::string>	_cgiPathes;
};

#endif
