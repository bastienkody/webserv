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
	//std::cout << "RAW:\n" << _raw << std::endl;
	_rql = RequestLine(_raw.substr(0, _raw.find("\r\n")));
	_raw.erase(0, _raw.find("\r\n") + 2);
	std::string	hTmp(_raw.substr(0, _raw.find("\r\n\r\n") + 2));
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
	_raw += data;
}

bool	Request::unchunkBody()
{
	std::string new_body, line;

	while (_body.size())
	{
		unsigned int		nb;
		std::stringstream	ss_nb(_body.substr(0, _body.find("\r\n")));
		ss_nb >> std::hex >> nb;
		if (ss_nb.fail() || !ss_nb.eof())
		{
			std::cout << "Illegal or missing hexadecimal sequence in chunked-encoding";
			return false;
		}

		_body.erase(0, _body.find("\r\n") + 2);
		line = _body.substr(0, _body.find("\r\n"));
		_body.erase(0, _body.find("\r\n") + 2);

		if (nb != line.size())
		{
			std::cout << "Wrong hexadecimal sequence in chunked-encoding with size of " << nb << " vs acutal data size " << line.size() << " for chunk data:" + line << std::endl; 
			return false;
		}

		if (nb) // to skip the len zero terminating chunk
			new_body.append(line);
	}
	_body = new_body;
	return true;
}
