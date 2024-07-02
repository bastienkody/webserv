#include "URL.hpp"

//	Default
URL::URL(){}
URL::~URL(){}
URL::URL(const URL & src) {*this = src;}
URL & URL::operator=(const URL & rhs)
{
	std::cout << "why is URL copy op called????" << std::endl;
	if(this != &rhs)
	{
		this->_fullURL = rhs.getFull();
		this->_protocol = rhs.getProtocol();
		this->_authority = rhs.getAuthority();
		this->_port = rhs.getPort();
		this->_path = rhs.getPath();
		this->_query = rhs.getQuery();
		this->_queries = rhs.getQueries();
		this->_anchor = rhs.getAnchor();
	}
	return (*this);
}
/*	getters	*/
std::string	const & URL::getFull() const {return _fullURL;}
std::string	const & URL::getProtocol() const {return _protocol;}
std::string	const & URL::getAuthority() const {return _authority;}
std::string const & URL::getPort() const {return _port;}
std::string	const & URL::getPath() const {return _path;}
std::string	const & URL::getQuery() const {return _query;}
std::multimap<std::string, std::string>	const & URL::getQueries() const {return _queries;}
std::string	const & URL::getAnchor() const {return _anchor;}
//	printers
void	URL::printDebug() const
{
	std::cout << "url_full:\t"<< this->getFull() << std::endl;
	std::cout << "url_proto:\t"<< this->getProtocol() << std::endl;
	std::cout << "url_authority:\t"<< this->getAuthority() << std::endl;
	std::cout << "url_port:\t"<< this->getPort() << std::endl;
	std::cout << "url_path:\t"<< this->getPath() << std::endl;
	std::cout << "url_query(str):\t" << this->getQuery() << std::endl;
	std::cout << "url_query(mmap):" << std::endl;
	for (std::multimap<std::string, std::string>::const_iterator it = _queries.begin(); it!=_queries.end(); ++it)
		std::cout << "\tentry=> " << it->first << ":" << it->second << std::endl;
	std::cout << "url_anchor:\t"<< this->getAnchor() << std::endl;
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
	// queries into str
	if (url.find(delimQuery) != std::string::npos)
	{
		_query = url.substr(url.find(delimQuery) + 1, url.size() - 1);
		url.erase(url.find(delimQuery), url.size() - 1);
		std::string qTmp(_query);
		// queries into multimap (pb if '=' in key or value )
		do
		{
			if (qTmp[0] == '&')
				qTmp.erase(0, 1);
			std::string	key(qTmp.substr(0, qTmp.find('=')));
			std::string	value(qTmp.substr(qTmp.find('=') + 1, (qTmp.find('&') != std::string::npos ? qTmp.find('&') - 2:qTmp.size())));
			_queries.insert(std::pair<std::string, std::string>(key, value));
			qTmp.erase(0, qTmp.find('&') != std::string::npos ? qTmp.find('&') : qTmp.size());

		} while (qTmp.find('&') != std::string::npos);
	}
	_path = url;
}