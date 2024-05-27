#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

#include<iostream>
#include<string>

class URL;

class RequestLine
{
	public:
		RequestLine();
		~RequestLine();
		RequestLine(const std::string name);
		RequestLine(const RequestLine & src);
		RequestLine & operator=(const RequestLine & rhs);
	private:
		std::string		fullRequestLine;
		std::string		method;
		class			Url;
};

std::ostream& operator<<(std::ostream& os, const RequestLine &rhs);

#endif
