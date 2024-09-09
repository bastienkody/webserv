#ifndef LOCATION_HPP
#define LOCATION_HPP

#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include "ConfigFile.hpp"
#include "../ParserUtils/ParserUtils.hpp"

class ConfigFile;

class Location: public ConfigFile
{
	public:
		Location();
		~Location();
		Location(const Location & src);
		Location & operator=(const Location & rhs);
	//	printer
		void	printAll() const;
	//	specific getters
		std::string const & getPath() const;
		bool		const & getIsPathAbsolute() const;
	//	specific setters
		void	setPath(std::string line);

		void	readInfos(std::string & raw); //throw
		bool	isValidElementLabel(std::string line);

	private:
		std::string	_path;
		bool		_isPathAbsolute;
};

#endif
