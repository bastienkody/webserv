#ifndef CLIENTS_H
#define CLIENTS_H

#include <vector>
#include "../Response/Response.hpp"
#include "../RequestParsing/Request.hpp"

struct client
{
	int fd;
	int	server_fd;
	bool	await_response;
	Request rq;
	Response rp;
};


int find_client(std::vector<struct client> all_co, int fd);

#endif
