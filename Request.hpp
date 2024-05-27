#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
#include<map>
#include<algorithm>

class RequestLine;

class Request
{
	public:
		Request();
		~Request();
		Request(const std::string name);
		Request(const Request & src);
		Request & operator=(const Request & rhs);
	private:
		class								RequestLine;
		std::map<std::string, std::string>	header;
		std::string							body;
};

std::ostream& operator<<(std::ostream& os, const Request &rhs);

#endif
