#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include "RequestLine.hpp"

class RequestLine;

class Request
{
	public:
		Request();
		~Request();
		Request(std::string rq);
		Request(const Request & src);
		Request & operator=(const Request & rhs);

		RequestLine							getRql() const;
		std::map<std::string, std::string>	getHeader() const;
		std::string							getBody() const;

		void	appendBody(std::string data);
		void	print() const;

	private:
		
		RequestLine							_rql;
		std::map<std::string, std::string>	_header;
		std::string							_body;
};

#endif
