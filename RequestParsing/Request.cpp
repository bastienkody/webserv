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
//	printer
void	Request::print() const
{
	std::cout << "verb:\t\t" << getRql().getVerb() << std::endl;
	std::cout << "version:\t" << getRql().getVersion() << std::endl;
	std::cout << "url_full:\t" << getRql().getUrl().getFull() << std::endl;
	std::cout << "url_proto:\t" << getRql().getUrl().getProtocol() << std::endl;
	std::cout << "url_autho:\t" << getRql().getUrl().getAuthority() << std::endl;
	std::cout << "url_port:\t" << getRql().getUrl().getPort() << std::endl;
	std::cout << "url_path:\t" << getRql().getUrl().getPath() << std::endl;
	std::cout << "url_query:\t" << getRql().getUrl().getQuery() << std::endl;
	std::cout << "url_anchor:\t" << getRql().getUrl().getAnchor() << std::endl;
	std::cout << "body:\t" << getBody() << std::endl;
}

//	Param constructor
Request::Request(std::string rq)
{
	_rql = RequestLine(rq.substr(0, rq.find("\n")));
	rq.erase(0, rq.find("\n"));
	std::string	headerTmp(rq.substr(0, rq.find("\n\n")));
	rq.erase(0, rq.find("\n\n"));
	// store header field with some particular ordred checks?
	_body = rq;
}

void	Request::appendBody(std::string data) 
{
	_body += data;
}