/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/13 14:35:15 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients/Clients.hpp"
#include "ConfigFile/ConfigFile.hpp"
#include "ConfigFile/Location.hpp"
#include "ConfigFile/Server.hpp"
#include "include/Poll.hpp"
#include "include/server.hpp"
#include <csignal>
#include <unistd.h>
#include <vector>

static bool run = true;

unsigned int *list_server_fd(Poll poll_fds)
{
	unsigned int *dest;

	dest = (unsigned int *)malloc(sizeof(unsigned int) * (poll_fds.getCount()));
	if (!dest)
		return (NULL);
	for (int i = 0; i < poll_fds.getCount(); i++)
		dest[i] = poll_fds.getFds(i).fd;
	return (dest);
}

void deco_client(std::vector<struct client> &clients, Poll *poll_fds, int i)
{
	int offset = find_client(clients, poll_fds->getFds(i).fd);
	if (DEBUGP) {std::cout << "[Client" << poll_fds->getFds(i).fd << "] to be deco" << std::endl;}
	if (offset > -1)
		clients.erase(clients.begin() + offset);
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

struct client	create_client(int server_fd, Poll &poll_fds)
{
	struct client cli;
	cli.fd = accept_new_connection(server_fd, &poll_fds);
	if (cli.fd < 0)
		run = false;
	cli.server_fd = server_fd;
	cli.await_response = false;
	return cli;
}

void	clear_client(struct client &cli)
{
	cli.await_response = false;
	cli.rp = Response();
	cli.rq = Request();
}

void	handler(__attribute__((unused))int dunmmy)
{
	std::cout << "... Webserv exiting ..." << std::endl;
	run = false;
	return;
}

void launch_server(ConfigFile config, Poll poll_fds)
{
	int				pos, status, bytes;
	std::vector<struct client>	clients;
	unsigned int			*server_fd = list_server_fd(poll_fds);
	if (server_fd == NULL)
		return;

	while (run)
	{
		if ( (status = poll_fds.call_to_poll()) < 0)
			return (free(server_fd), close_clients(clients), perror("poll"));
/*		else if (status == 0) // selon la doc on devrait jamais avoir  ca
		{
			std::cout << "Status == 0 in poll !!" << std::endl;
			continue;
		}*/
		for (int i = 0; i < poll_fds.getCount(); i++)
		{
			pos = find_client(clients, poll_fds.getFds(i).fd);
			if (poll_fds.getFds(i).revents & POLLIN)
			{
				if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd, config.getServers().size())) != -1)
					clients.push_back(create_client(server_fd[status], poll_fds));
				else
				{
					bytes = read_recv_data(i, &poll_fds, clients[pos]);
					if (bytes <= 0)
						deco_client(clients, &poll_fds, i);
					else if (is_rq_finished(clients[pos].rq.getRaw()))
						clients[pos].await_response = true;
				}
			}
			else if (poll_fds.getFds(i).revents & POLLOUT && pos > -1 && clients[pos].await_response == true)
			{
				if (send_response(clients[pos], config) < 0 || RequestChecking::isKeepAlive(clients[pos].rq) == false)
					deco_client(clients, &poll_fds, i); // pb de read/write ou no keepalive  --> deco client
				else
					clear_client(clients[pos]);
			}
		}
	}
	free(server_fd);
	close_clients(clients);
}

int verif_host(ConfigFile config, int i)
{
	for (int y = 0; y < i; y++)
		if (config.getServers()[y].getIp() == config.getServers()[i].getIp() && config.getServers()[y].getPort() == config.getServers()[i].getPort())
			return (1);
	return (0);
}

int main(int ac, char **av)
{
	signal(SIGINT, handler);

	if (ac > 2)
		return std::cerr << "expected config file as single argument" << std::endl, 2;

	ConfigFile config(ac == 2 ? av[1] : "ConfigFile/files/new.config");
	try {
		config.openReadFileToStr();
		config.readAllInfos();
		//config.printAll();
		if (config.getServers().size() == 0)
			return std::cerr<<"No server in config file"<<std::endl, 1;
	}
	catch (const std::exception &e) {
		return std::cerr << e.what() << std::endl, 1;
	}
	Poll poll_fds;
	for (size_t i = 0; i < config.getServers().size(); i++)
	{
		if (verif_host(config, i) == 1)
			continue;
		int fd = create_socket_server(config.getServers()[i]);
		if (fd == -1)
			return poll_fds.end_close_fd(), 1;
		try {
			poll_fds.add_to_poll(fd);
			config.setServerFd(fd, i);
		}
		catch (const std::exception &e) {
			return std::cerr << e.what() << std::endl, poll_fds.end_close_fd(), 1;
		}
	}
	launch_server(config, poll_fds);
	poll_fds.end_close_fd();
	return 0;
}
