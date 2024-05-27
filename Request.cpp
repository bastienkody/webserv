#include"Request.hpp"

//	Default constructor
Request::Request()
{
}

//	Default destructor
Request::~Request()
{
}

//	Param constructor
Request::Request(const std::string name)
{
}

//	Copy constructor (via copy operator)
Request::Request(const Request & src)
{
	*this = src;
}

//	Copy operator
Request & Request::operator=(const Request & rhs)
{
	if(this != &rhs)
	{}
	return (*this);
}

//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const Request &rhs)
{
	return (os << rhs.getName());
} 

