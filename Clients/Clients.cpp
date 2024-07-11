#include "Clients.hpp"

// bad function bc it is stacked allocated
std::vector<struct client>::iterator find_co_by_fd_it(std::vector<struct client> all_co, int fd)
{
	std::vector<struct client>::iterator it = all_co.begin();
	std::vector<struct client>::iterator ite = all_co.end();

	for(; it!=ite; ++it)
		if (fd == it->fd)
			 return std::cout << "from find by it, fd:" << it->fd << std::endl, it;
	return ite;
}

int find_co_by_fd_pos(__attribute__((unused))std::vector<struct client> all_co, int fd)
{
	for(unsigned int i = 0; i < all_co.size(); ++i)
		if (fd == all_co[i].fd)
			return i;
	return -1;
}
