#include"Request.hpp"

//	Default
Request::Request(){}
Request::~Request(){}
Request::Request(const Request & src) {*this = src;}
//	Copy operator
Request & Request::operator=(const Request & rhs)
{
	if(this != &rhs)
	{
		this->_rql = rhs.getRql();
		this->_header = rhs.getHeader();
		this->_body = rhs.getBody();
	}
	return (*this);
}
//	getters
RequestLine							Request::getRql() const 	{return _rql;}
std::map<std::string, std::string>	Request::getHeader() const	{return _header;}
std::string							Request::getBody() const	{return _body;} 
//	printer
void	Request::print() const
{
	std::cout << "verb:\t\t" << getRql().getVerb() << std::endl;
	std::cout << "version:\t" << getRql().getVersion() << " (bool:" << (getRql().getIsVersionGood() == 1 ? "true" : "false")  << ")" << std::endl;
	getRql().getUrl().printDebug();
	std::cout << "body:\t" << getBody() << std::endl;
}

//	Param constructor
Request::Request(std::string rq)
{
	_rql = RequestLine(rq.substr(0, rq.find("\r\n")));
	rq.erase(0, rq.find("\r\n"));
	std::string	headerTmp(rq.substr(0, rq.find("\n\n")));
	rq.erase(0, rq.find("\n\n"));
	// store header field with some particular ordred checks? not for now : multimap

	_body = rq;
}

void	Request::appendBody(std::string data) 
{
	_body += data;
}
