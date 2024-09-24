#include "Clients.hpp"

int find_client(std::vector<struct client> clients, int fd)
{
	for(unsigned int i = 0; i < clients.size(); ++i)
		if (fd == clients[i].fd)
			return i;
	return -1;
}

void	clear_client(struct client &cli)
{
	cli.await_response = false;
	cli.rp = Response();
	cli.rq = Request();
}

void deco_client(std::vector<struct client> &clients, Poll *poll_fds, int i)
{
	int offset = find_client(clients, poll_fds->getFds(i).fd);
	if (DEBUGP) {std::cout << "[Client" << poll_fds->getFds(i).fd << "] to be deco" << std::endl;}
	if (offset > -1)
		clients.erase(clients.begin() + offset);
	if (poll_fds->getFds(i).fd > 0)
		close(poll_fds->getFds(i).fd);
	poll_fds->remove_to_poll(i);
}

void	close_clients(std::vector<struct client> &clients)
{
	for (std::vector<struct client>::iterator it = clients.begin(); it != clients.end(); ++it)
		if (it->fd > 0)
			close(it->fd);
	clients.clear();
}


