#ifndef REQUESTCHECKING_HPP
#define REQUESTCHECKING_HPP

#include<iostream>
#include<fstream>
#include<string>
#include "../RequestParsing/Request.hpp"
#include "../ParserUtils/ParserUtils.hpp"

#define	EXPECTED_VERSION "HTTP/1.1"
#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"

class Request;

class RequestChecking
{
	public:
		static int	CheckBasics(const Request & rq);
		static int	CheckHeaderKey(const Request & rq);
		static int	CheckRequiredHeaderPOST(const Request & rq, std::string max_body_size);
		static int	CheckRequiredHeaderDELETE(const Request & rq);
		static int	CheckRequiredHeaderGET(const Request & rq);
		static bool	isKeepAlive(const Request & rq);

};

std::ostream& operator<<(std::ostream& os, const RequestChecking &rhs);

#endif
