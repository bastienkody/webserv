#include"URL.hpp"

//	Default constructor
URL::URL()
{

}

//	Default destructor
URL::~URL()
{
}

//	Param constructor
URL::URL(const std::string str)
{
	_fullURL = str;
	if (str.find("https://") == std::string::npos && str.find("http://") == 0)
		absoluteFormParser();
	else
		originFormParser(str); // on laisse les https ici (nsp si necessaire)
}

//	Copy constructor (via copy operator)
URL::URL(const URL & src)
{
	*this = src;
}

//	Copy operator
URL & URL::operator=(const URL & rhs)
{
	if(this != &rhs)
	{
		this->_fullURL = rhs.getFull();
		this->_protocol = rhs.getProtocol();
		this->_port = rhs.getPort();
		this->_path = rhs.getPath();
		this->_queries = rhs.getQueries();
		this->_anchor = rhs.getAnchor();
	}
	return (*this);
}

/*	ie.  http://www.example.com:80/path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument	
	port can be ommited, query and anchor too
*/
void	URL::absoluteFormParser()
{
	std::string			url(_fullURL), delimProto = "://", delimPort = ":", delimEndDomain;
	std::stringstream	sstmp;

	// "https://" remove
	url.erase(0, url.find(delimProto) + delimProto.size());
	//	domain (until port or path)
	url.find(delimPort) == std::string::npos ? delimEndDomain = "/" : ":";
	_authority = url.substr(0, url.find(delimEndDomain));
	url.erase(0, url.find(delimEndDomain));
	// port?
	if (delimEndDomain == delimPort)
	{
		sstmp << url.substr(url.find(delimPort) + 1, url.find("/"));
		url.erase(url.find(delimPort) + 1, url.find("/"));
		sstmp >> _port;
		if (sstmp.fail() || !sstmp.eof())
			throw std::invalid_argument("bad port in aboslute url form?");
	}
	originFormParser(url);
}

/*	ie.  /path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument
	query and anchor can be ommited
	split query here (or in cgi???)
	it takes an url argument here to also work with the absolute already treated
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
		std::string	query = url.substr(url.find(delimQuery) + 1, url.size() - 1);
		url.erase(url.find(delimQuery), url.find(delimQuery) + 1);
		// key1=value1&key2=value2 etc
		do	{
			std::string	key(query.substr(0, query.find("=") - 1));
			std::string	value(query.substr(query.find("=") + 1), query.find("&") == std::string::npos ? query.size() -1 : query.find("&"));
			_queries[key] = value;
			query.erase(0, query.find("&") == std::string::npos ? query.size() -1 : query.find("&") - 1);
		}	while (query.find("&") != std::string::npos);
	}
	_path = url;
}

/*	getters	*/
std::string	URL::getFull() const
{
	return _fullURL;
}
std::string	URL::getProtocol() const
{
	return _protocol;
}
std::string	URL::getAuthority() const
{
	return _authority;
}
unsigned int	URL::getPort() const
{
	return _port;
}
std::string	URL::getPath() const
{
	return _path;
}
std::map<std::string, std::string>	URL::getQueries() const
{
	return _queries;
}
std::string	URL::getAnchor() const
{
	return _anchor;
}

//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const URL &rhs)
{
	return (os << rhs.getName());
} 

