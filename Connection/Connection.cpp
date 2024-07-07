#include"Connection.hpp"

//	Default constructor
Connection::Connection() {}
Connection::~Connection() {}

//	Param constructor
Connection::Connection(int socket_fd): _fd(socket_fd) {}

//	Copy constructor (via copy operator)
Connection::Connection(const Connection & src) {*this = src;}
Connection & Connection::operator=(const Connection & rhs)
{
	if(this != &rhs)
	{
		std::cout << "copy op connection" << std::endl;
		std::cout << "with fd:" << rhs.getFd() << std::endl;
		std::cout << "with rqraw:" << rhs._rq.getRaw() << std::endl;
		_fd = rhs._fd;
		_rq = rhs._rq;
		_rp = rhs._rp;
	}
	return (*this);
}

//	Getter (rp and rq not const ref because theyll be modified)
int const & Connection::getFd() const {return (_fd);}
Request 	Connection::getRq()  {return (_rq);}
Response 	Connection::getRp()  {return (_rp);}
