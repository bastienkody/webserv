#ifndef CLIENTS_H
#define CLIENTS_H

#include <vector>
#include "../Response/Response.hpp"
#include "../RequestParsing/Request.hpp"
#include "../include/Poll.hpp"
#include "../include/server.hpp"

struct client
{
	int		fd;
	int		server_fd;
	bool		await_response;
	Request		rq;
	Response	rp;
};

int	find_client(std::vector<struct client> all_co, int fd);
void	clear_client(struct client &cli);
void	deco_client(std::vector<struct client> &clients, Poll *poll_fds, int i);
void	close_clients(std::vector<struct client> &clients);

#endif
