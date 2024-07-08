#ifndef CLIENTS_H
#define CLIENTS_H

#include <vector>
#include "../Response/Response.hpp"
#include "../RequestParsing/Request.hpp"

struct client
{
	int fd;
	int	serv_sock;
	Request rq;
	Response rp;
};


std::vector<struct client>::iterator find_co_by_fd_it(__attribute__((unused))std::vector<struct client> all_co, int fd);
int find_co_by_fd_pos(__attribute__((unused))std::vector<struct client> all_co, int fd);

#endif
