#include"RequestLine.hpp"

//	Default constructor
RequestLine::RequestLine()
{
}

//	Default destructor
RequestLine::~RequestLine()
{
}

//	Param constructor
RequestLine::RequestLine(const std::string name)
{
}

//	Copy constructor (via copy operator)
RequestLine::RequestLine(const RequestLine & src)
{
	*this = src;
}

//	Copy operator
RequestLine & RequestLine::operator=(const RequestLine & rhs)
{
	if(this != &rhs)
	{}
	return (*this);
}

//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const RequestLine &rhs)
{
	return (os << rhs.getName());
} 

