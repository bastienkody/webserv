#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include<iostream>
#include<string>
#include "../RequestParsing/Request.hpp"
#include "../Response/Response.hpp"

class Connection
{
	public:

		Connection();
		~Connection();
		Connection(int socket_fd);
		Connection(const Connection & src);
		Connection & operator=(const Connection & rhs);

		int const & getFd() const;
		Request getRq();
		Response getRp();

	private:

		int			_fd;
		Request		_rq;
		Response	_rp;
};

#endif
