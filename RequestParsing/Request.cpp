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
	std::cout << "verb:\t\t" << getRql().getVerb() << std::endl;
	std::cout << "version:\t" << getRql().getVersion() << std::endl;
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
	std::string	hTmp(rq.substr(0, rq.find("\n\n") + 1));
	rq.erase(0, rq.find("\n\n") + 2);

	while (hTmp.find('\n') != std::string::npos)
	{
		std::string	line(hTmp.substr(0, hTmp.find('\n')));
		hTmp.erase(0, hTmp.find('\n') + 1);

		if (ParserUtils::trimOWS(line).size() == 0 || line.find(':') == std::string::npos) // on ignore les header sans ':'
			continue;

		std::string key = line.substr(0, line.find(':'));
		std::string	value = ParserUtils::trimOWS(line.substr(line.find(':') + 1, line.size() - 1));
		std::cout << "line=" + line + "|store=" + key + "-->" + value << std::endl;
		_header.insert(std::pair<std::string, std::string>(key, value));
	}
	_body = rq;
}

void	Request::appendBody(std::string data) 
{
	_body += data;
}
