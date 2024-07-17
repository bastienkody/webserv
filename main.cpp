/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/16 14:53:02 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*---------TO DO---------
- : pas encore fait/en cours
~ : fait mais pas sur que ce soit bien
+ : fait



RECOMMENCER MERGE (AJOUTER Poll A CONFIGFILE OBJ + refaire makefile)

        ~ finir mise en place serv test
                ~ creer class poll pour faciliter manip (notamment l'update du
pollfd) puis finir implementation
                - creer class socket (je sais pas encore comment je vais m'en
servir) ~ passer socket en non bloquant
                + POUVOIR ECOUTER SUR PLUSIEURS PORTS (creer socket server pour
chaque port ?) ~ gerer requete chunk
        - METTRE EN PLACE CGI
        - gerer ecriture sur fd via poll()
        - utiliser "signal" pour catch ctrl c et terminer le programme
        - mettre a la norme du projet (check fonction autorise et alternative
pour fonctions C)
        - gerer leaks et fds
-------------------------*/

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
	int offset = find_co_by_fd_pos(clients, poll_fds->getFds(i).fd);
	if (offset > -1)
		clients.erase(clients.begin() + offset);
	close(poll_fds->getFds(i).fd);
	poll_fds->remove_to_poll(i);
}

void launch_server(ConfigFile config, Poll poll_fds)
{
	unsigned int *server_fd = list_server_fd(poll_fds);
	std::vector<struct client> clients;
	std::string buff;
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
				// new client requesting the server
				if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd, poll_fds.getCount())) != -1)
				{
					struct client cli;
					cli.fd = accept_new_connection(server_fd[status], &poll_fds);
					cli.server_fd = poll_fds.getFds(i).fd;
					cli.await_response = false;
					clients.push_back(cli);
				}
				// data to read from the client request
				else
				{
					try {
						buff = read_recv_data(i, &poll_fds);
						if ((pos = find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)) == -1) 
							std::cout << "pos == -1 in pollin read recv" << std::endl; // should never happen, to be removed later
						clients[pos].rq.appendRaw(buff);
						clients[pos].await_response = true;
					}
					catch (const std::exception & e) {
						deco_client(clients, &poll_fds, i);
					}
				}
			}
			// responding
			else if (poll_fds.getFds(i).revents & POLLOUT && clients.size() > 0 && clients[find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)].await_response == true)
			{
				if (send_response(clients[find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)], config) < 0)
					deco_client(clients, &poll_fds, i); // pb de read/write --> deco client
				else
					clients[find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)].await_response = false;
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

int main(int ac, char **av)
{
	if (ac > 2)
		return std::cerr << "expected config file as single argument" << std::endl, 2;

	ConfigFile config(ac == 2 ? av[1] : "ConfigFile/files/new.config");
	try {
	config.openReadFileToStr();
	config.readAllInfos();
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
