#include "Clients.hpp"

int find_client(std::vector<struct client> clients, int fd)
{
	for(unsigned int i = 0; i < clients.size(); ++i)
		if (fd == clients[i].fd)
			return i;
	return -1;
}
