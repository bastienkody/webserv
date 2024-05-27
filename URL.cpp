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
	if (std::string(str.begin(), str.begin() + 8).compare("https://"))
		// bad protocol
	if (std::string(str.begin(), str.begin() + 7).compare("http://"))
		absoluteFromParser();
	else
		_path = str;
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
	port can be ommited
*/
void	URL::absoluteFromParser()
{
	std::string delim[] = {"://", ":", "?", "#"};
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

