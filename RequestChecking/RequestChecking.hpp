#ifndef REQUESTCHECKING_HPP
#define REQUESTCHECKING_HPP

#include<iostream>
#include<string>
#include "../RequestParsing/Request.hpp"

class Request;

class RequestChecking
{
	public:
		static	bool CheckBasics(Request rq);

};

std::ostream& operator<<(std::ostream& os, const RequestChecking &rhs);

#endif
