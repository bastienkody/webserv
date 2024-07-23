#include"Request.hpp"
#include <cstring>
#include <stdexcept>

//	Default
Request::Request(){}
Request::~Request(){}
Request::Request(const Request & src) {*this = src;}
//	Copy operator
Request & Request::operator=(const Request & rhs)
{
	if(this != &rhs)
	{
		this->_raw = rhs.getRaw();
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
std::string							const & Request::getRaw() const	{return _raw;}
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

//	Parse into rq line, headers and body
void	Request::parse()
{
	_rql = RequestLine(_raw.substr(0, _raw.find("\r\n")));
	_raw.erase(0, _raw.find("\r\n") + 2);
	std::string	hTmp(_raw.substr(0, _raw.find("\r\n\r\n")));
	_raw.erase(0, _raw.find("\r\n\r\n") + 4);

	while (hTmp.find('\n') != std::string::npos)
	{
		std::string	line(hTmp.substr(0, hTmp.find('\n')));
		hTmp.erase(0, hTmp.find('\n') + 1);

		if (ParserUtils::trimOWS(line).size() == 0 || line.find(':') == std::string::npos) // on ignore les header sans ':'
			continue;

		std::string key = line.substr(0, line.find(':'));
		std::string	value = ParserUtils::trimOWS(line.substr(line.find(':') + 1, line.size() - 1));
		//std::cout << "line=" + line + "|store=" + key + "-->" + value << std::endl;
		_header.insert(std::pair<std::string, std::string>(key, value));
	}
	_body = _raw;
}

void	Request::appendRaw(std::string data) 
{
	//_raw.reserve(_raw.size() + data.size());
	// chumk must be parsed specifically
	_raw += data;
}

void	Request::unchunkBody()
{
	
}
