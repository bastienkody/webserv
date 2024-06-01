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
RequestLine							const & Request::getRql() const 	{return _rql;}
std::map<std::string, std::string>	const & Request::getHeader() const	{return _header;}
std::string							const & Request::getBody() const	{return _body;} 
//	printer
void	Request::print() const
{
	std::cout << "verb:\t\t" << getRql().getVerb() << " (bool:" << (getRql().getIsVerbGood() == 1 ? "true" : "false")  << ")" << std::endl;
	std::cout << "version:\t" << getRql().getVersion() << " (bool:" << (getRql().getIsVersionGood() == 1 ? "true" : "false")  << ")" << std::endl;
	getRql().getUrl().printDebug();
	std::cout << "header:" << std::endl;
	for (std::multimap<std::string, std::string>::const_iterator it = _header.begin(); it!=_header.end(); ++it)
		std::cout << "\tentry=> " << it->first << ":" << it->second << std::endl;
	std::cout << "body:\t" << getBody() << std::endl;
}

//	Param constructor
Request::Request(std::string rq)
{
	_rql = RequestLine(rq.substr(0, rq.find("\r\n")));
	rq.erase(0, rq.find("\r\n") + 2);
	std::string	hTmp(rq.substr(0, rq.find("\n\n")));
	rq.erase(0, rq.find("\n\n") + 2);
	do
	{
		if (hTmp[0] == '\n')
			hTmp.erase(0, 1);
		std::string	value(hTmp.substr(hTmp.find(':') + 1, (hTmp.find('\n') != std::string::npos ? hTmp.find('\n') - 5:hTmp.size())));
		_header.insert(std::pair<std::string, std::string>(hTmp.substr(0, hTmp.find(':')), value));
		hTmp.erase(0, hTmp.find('\n') != std::string::npos ? hTmp.find('\n') : hTmp.size());
	} while (hTmp.find('\n') != std::string::npos);
	_body = rq;
}

void	Request::appendBody(std::string data) 
{
	_body += data;
}
