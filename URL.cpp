#include "URL.hpp"

//	Default
URL::URL(){}
URL::~URL(){}
URL::URL(const URL & src) {*this = src;}
URL & URL::operator=(const URL & rhs)
{
	if(this != &rhs)
	{
		this->_fullURL = rhs.getFull();
		this->_protocol = rhs.getProtocol();
		this->_port = rhs.getPort();
		this->_path = rhs.getPath();
		this->_query = rhs.getQuery();
		this->_anchor = rhs.getAnchor();
	}
	return (*this);
}
/*	getters	*/
std::string	URL::getFull() const {return _fullURL;}
std::string	URL::getProtocol() const {return _protocol;}
std::string	URL::getAuthority() const {return _authority;}
std::string	URL::getPort() const {return _port;}
std::string	URL::getPath() const {return _path;}
std::string	URL::getQuery() const {return _query;}
std::string	URL::getAnchor() const {return _anchor;}
//	printers
void	URL::printDebug() const
{
	std::cout << "full URL\t:\t"<< this->getFull() << std::endl;
	std::cout << "---------------------------------------------"<<  std::endl;
	std::cout << "protocol\t:\t"<< this->getProtocol() << std::endl;
	std::cout << "authority\t:\t"<< this->getAuthority() << std::endl;
	std::cout << "port\t\t:\t"<< this->getPort() << std::endl;
	std::cout << "path\t\t:\t"<< this->getPath() << std::endl;
	std::cout << "query\t\t:\t" << this->getQuery() << std::endl;
	std::cout << "anchor\t\t:\t"<< this->getAnchor() << std::endl;
}
//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const URL &rhs) 
{return (os << rhs.getFull());} 


/*
	What we really use
*/

//	Param constructor
URL::URL(const std::string str)
{
	_fullURL = str;
	absoluteFormParser();
}

/*	ie.  http://www.example.com:80/path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument	
	port can be ommited / 
	port overflow must be handled without exception? bad port char can be checked later on
*/
void	URL::absoluteFormParser()
{
	std::string			url(_fullURL), dProto = "://", dPort = ":", dEndDomain;

	// protocol
	if (url.find(dProto) != std::string::npos)
	{
		_protocol = url.substr(0, url.find(dProto));
		url.erase(0, url.find(dProto) + dProto.size());
	}
	//	domain
	dEndDomain = (url.find(dPort) == std::string::npos ?  "/" : ":");
	_authority = url.substr(0, url.find(dEndDomain));
	url.erase(0, url.find(dEndDomain));
	// port?
	if (dEndDomain == dPort)
	{
		_port = url.substr(url.find(dPort) + 1, url.find("/") - 1);
		url.erase(url.find(dPort), url.find("/"));
	}
	originFormParser(url);
}

/*	ie.  /path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument
	query and anchor can be ommited
	split query here (or in cgi???)
*/
void	URL::originFormParser(std::string url)
{
	std::string	delimPath="/", delimQuery="?", interQ="&", delimAnchor="#";
	std::string	endPath;

	// anchor fragment
	if (url.find(delimAnchor) != std::string::npos)
	{
		_anchor = url.substr(url.find(delimAnchor) + 1, url.size() - 1);
		url.erase(url.find(delimAnchor), url.size() - 1);
	}
	// queries into map
	if (url.find(delimQuery) != std::string::npos)
	{
		_query = url.substr(url.find(delimQuery) + 1, url.size() - 1);
		url.erase(url.find(delimQuery), url.size() - 1);
	}
	_path = url;
}