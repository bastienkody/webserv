/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/10 14:33:27 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients/Clients.hpp"
#include "ConfigFile/ConfigFile.hpp"
#include "ConfigFile/Location.hpp"
#include "ConfigFile/Server.hpp"
#include "include/Poll.hpp"
#include "include/server.hpp"
#include <unistd.h>
#include <vector>

unsigned int *list_server_fd(Poll poll_fds)
{
	unsigned int *dest;

	dest = (unsigned int *)malloc(sizeof(unsigned int) * (poll_fds.getCount()));
	if (!dest)
		return (0);
	for (int i = 0; i < poll_fds.getCount(); i++)
		dest[i] = poll_fds.getFds(i).fd;
	return (dest);
}

void deco_client(std::vector<struct client> &clients, Poll *poll_fds, int i)
{
	int offset = find_client(clients, poll_fds->getFds(i).fd);
	std::cout << "[Client" << poll_fds->getFds(i).fd << "] to be deco" << std::endl;
	if (offset > -1)
		clients.erase(clients.begin() + offset);
	close(poll_fds->getFds(i).fd);
	poll_fds->remove_to_poll(i);
}

struct client	create_client(int server_fd, Poll &poll_fds)
{
	struct client cli;
	cli.fd = accept_new_connection(server_fd, &poll_fds);
	cli.server_fd = server_fd;
	cli.await_response = false;
	return cli;
}

void launch_server(ConfigFile config, Poll poll_fds)
{
	unsigned int *server_fd = list_server_fd(poll_fds);
	std::vector<struct client> clients;
	int	pos;

	while (true)
	{
		int status = poll_fds.call_to_poll();
		if (status < 0)
			return (perror("poll"));
		else if (status == 0)
			continue;
		for (int i = 0; i < poll_fds.getCount(); i++)
		{
			if (poll_fds.getFds(i).revents & POLLIN)
			{
				std::cout << "pi_" << std::endl;
				// new client requesting the server
				if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd, poll_fds.getCount())) != -1)
					clients.push_back(create_client(server_fd[status], poll_fds));
				// data to read from the client request
				else
				{
					try {
						pos = find_client(clients, poll_fds.getFds(i).fd);
						read_recv_data(i, &poll_fds, clients[pos]);
						clients[pos].await_response = true;
						usleep(20);
					}
					catch (const std::exception & e) {
						deco_client(clients, &poll_fds, i);
					}
					usleep(200);
				}
			}
			// responding
			else if (poll_fds.getFds(i).revents & POLLOUT && clients.size() > 0 && clients[find_client(clients, poll_fds.getFds(i).fd)].await_response == true)
			{
				std::cout << "po_" << std::endl;
				pos = find_client(clients, poll_fds.getFds(i).fd);
				if (send_response(clients[pos], config) < 0 || RequestChecking::isKeepAlive(clients[pos].rq) == false)
					deco_client(clients, &poll_fds, i); // pb de read/write ou no keepalive  --> deco client
				else
				{
					clients[pos].await_response = false;
					clients[pos].rp = Response();
					clients[pos].rq = Request();
			    }
			}
		}
	}
	free(server_fd);
}

int verif_host(ConfigFile config, int i)
{
	for (int y = 0; y < i; y++)
		if (config.getServers()[y].getIp() == config.getServers()[i].getIp() && config.getServers()[y].getPort() == config.getServers()[i].getPort())
			return (1);
	return (0);
}

int main(int ac, char **av, __attribute__((unused))char **env)
{
	if (ac > 2)
		return std::cerr << "expected config file as single argument" << std::endl, 2;

	ConfigFile config(ac == 2 ? av[1] : "ConfigFile/files/new.config");
	try {
		config.openReadFileToStr();
		config.readAllInfos();
		//config.printAll();
	}
	catch (const std::exception &e) {
		return std::cerr << e.what() << std::endl, 1;
	}
	Poll poll_fds;
	for (size_t i = 0; i < config.getServers().size(); i++)
	{
		if (verif_host(config, i) == 1)
			continue;
		int fd = create_socket_server(config.getServers()[i].getPortSTR().c_str());
		if (fd == -1)
			return 1;
		try {
			poll_fds.add_to_poll(fd);
			config.setServerFd(fd, i); // for clients to be linked to a their own server (ie maxbodysize on that specific server)
		}
		catch (const std::exception &e) {
			return std::cerr << e.what() << std::endl, 1;
		}
	}
	launch_server(config, poll_fds);
	return 0;
}
