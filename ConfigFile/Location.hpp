#ifndef LOCATION_HPP
#define LOCATION_HPP

#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include "../ParserUtils/ParserUtils.hpp"

class Location
{
	public:
		Location();
		~Location();
		Location(const Location & src);
		Location & operator=(const Location & rhs);
	//	printer
		void	printAll() const;
	//	getters
		std::string const & getPath() const;
		bool		const & getIsPathAbsolute() const;
		std::string const & getRoot() const;
		std::string const & getIndex() const;
		std::string const & getAutoIndex() const;
		std::string const & getMaxBodySize() const;
		std::string const & getRedirection() const;
		std::map<std::string, std::string> const & getErrorPages() const;
		std::vector<std::string> const & getAllowMethods() const;
		std::vector<std::string> const & getCgiPathes() const;
		std::vector<std::string> const & getCgiExt() const;
	//	setters
		void	setPath(std::string line);
		void	setRoot(std::string line);
		void	setIndex(std::string line);
		void	setAutoIndex(std::string line);
		void	setMaxBodySize(std::string line);
		void	setAllowMethods(std::string line);
		void	setRedirection(std::string line);
		void	setErrorPages(std::string line);
		void	setCgiPathes(std::string line);
		void	setCgiExt(std::string line);

		void	readInfos(std::string & raw); //throw
		bool	isValidElementLabel(std::string line);

	private:
		std::string	_path;
		bool		_isPathAbsolute;
		std::string _root;
		std::string	_index;
		std::string	_autoindex;
		std::string	_maxBodysize;	//stored as a string but ovlf checks needed when reading + use of neg (not zero) if not provided
		std::string	_redirection;
		// upload_files ?? i dont understand how nginx deals with it ; may we do smthg different?
		std::vector<std::string>	_cgiExt;
		std::map<std::string,std::string>	_errorPages;
		std::vector<std::string>	_allowMethods;
		std::vector<std::string>	_cgiPathes;
		
};

#endif
