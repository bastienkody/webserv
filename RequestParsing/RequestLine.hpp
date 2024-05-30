#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

#include<iostream>
#include<string>
#include "URL.hpp"

class URL;

class RequestLine
{
	public:
		RequestLine();
		~RequestLine();
		RequestLine(const std::string name);
		RequestLine(const RequestLine & src);
		RequestLine & operator=(const RequestLine & rhs);

		std::string	getFull() const;
		std::string	getVerb() const;
		std::string	getVersion() const;
		URL			getUrl() const;

	private:
		std::string		_fullRequestLine;
		std::string		_verb;
		std::string		_version;
		URL				_Url;
		
};

std::ostream& operator<<(std::ostream& os, const RequestLine &rhs);

#endif
