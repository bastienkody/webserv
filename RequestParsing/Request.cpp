#include"Request.hpp"

//	Default
Request::Request(){}
Request::~Request(){}
Request::Request(const Request & src) {*this = src;}
//	Copy operator
Request & Request::operator=(const Request & rhs)
{
	if(this != &rhs)
	{}
	return (*this);
}
//	getters
RequestLine							Request::getRql() const 	{return _rql;}
std::map<std::string, std::string>	Request::getHeader() const	{return _header;}
std::string							Request::getBody() const	{return _body;} 

//	Param constructor
Request::Request(std::string rq)
{
	_rql = RequestLine(rq.substr(0, rq.find(10)));
	rq.erase(0, rq.find(10));
	std::string	headerTmp(rq.substr(0, rq.find("\n\n")));
	rq.erase(0, rq.find("\n\n"));
	// store header field with some particular ordred checks?
	_body = rq;
}

void	Request::appendBody(std::string data) 
{
	_body += data;
}