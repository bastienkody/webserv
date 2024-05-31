#include"RequestLine.hpp"

//	Default
RequestLine::RequestLine() {}
RequestLine::~RequestLine() {}
RequestLine::RequestLine(const RequestLine & src) {*this = src;}
RequestLine & RequestLine::operator=(const RequestLine & rhs)
{
	if(this != &rhs)
	{
		this->_fullRequestLine = rhs.getFull();
		this->_verb = rhs.getVerb();
		this->_Url = rhs.getUrl();
	}
	return (*this);
}
// getters
std::string		RequestLine::getFull() const {return _fullRequestLine;}
std::string		RequestLine::getVerb() const {return _verb;}
std::string		RequestLine::getVersion() const {return _version;}
URL				RequestLine::getUrl() const {return _Url;}
//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const RequestLine &rhs)
{ return (os << rhs.getFull()); } 


//	Param constructor
/*
	VERB WS URL WS VERSION CRLF
	GET / HTTP/1.1
	POST http://exampleform.com/home/login/form HTTP/1.1
	DELETE /thatfile.html HTTP/1.1
	*/
RequestLine::RequestLine(std::string rql)
{
	_fullRequestLine = rql;
	_verb = rql.substr(0, rql.find(" "));
	rql.erase(0, rql.find(" ") + 1);
	_version = rql.substr(rql.rfind(" "), rql.find("\r\n") - 2);
	std::cout << "version:" << _version << std::endl;
	rql.erase(rql.rfind(" "), rql.find("\r\n"));
	_Url = URL(rql);
}
