#ifndef LOCATION_HPP
#define LOCATION_HPP

#include<iostream>
#include<string>
#include<string>
#include<vector>
#include<map>
#include "../ParserUtils/ParserUtils.hpp"

class Location
{
	public:
		//Location(const std::string name);
		//Location(const Location & src);
		Location & operator=(const Location & rhs);

		void	readInfos(std::string & raw); //throw

	private:
		std::string _root;
		std::string	_index;
		std::string	_autoindex;
		std::string	_maxBodysize;	//stored as a string but ovlf checks needed when reading + use of neg (not zero) if not provided
		std::string	_allowMethods;
		std::map<std::string,std::string> _errorPages;
		std::vector<std::string> _cgiPathes;
		std::vector<std::string> _cgiExt;
		
};

#endif
