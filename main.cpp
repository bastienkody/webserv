/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/04 13:16:17 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*---------TO DO---------
- : pas encore fait/en cours
~ : fait mais pas sur que ce soit bien
+ : fait



RECOMMENCER MERGE (AJOUTER Poll A CONFIGFILE OBJ + refaire makefile)

	~ finir mise en place serv test
		~ creer class poll pour faciliter manip (notamment l'update du pollfd) puis finir implementation
		- creer class socket (je sais pas encore comment je vais m'en servir)
		~ passer socket en non bloquant
		+ POUVOIR ECOUTER SUR PLUSIEURS PORTS (creer socket server pour chaque port ?)
	~ gerer requete chunk
	- METTRE EN PLACE CGI
	- gerer ecriture sur fd via poll()
	- utiliser "signal" pour catch ctrl c et terminer le programme
	- mettre a la norme du projet (check fonction autorise et alternative pour fonctions C)
	- gerer leaks et fds
-------------------------*/

#include "ConfigFile/ConfigFile.hpp"
#include "ConfigFile/Server.hpp"
#include "ConfigFile/Location.hpp"
#include "Connection/Connection.hpp"
#include "include/Poll.hpp"
#include "include/server.hpp"
#include <unistd.h>
#include <vector>

static std::string rep("HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 5\nContent-Type: text/html\nConnection: Keep-alive\n\nBody\n");

unsigned int *list_server_fd(Poll poll_fds)
{
	unsigned int *dest;

	dest = (unsigned int *) malloc(sizeof(unsigned int) * (poll_fds.getCount()));
	if (!dest)
		return (0);
	for (int i = 0; i < poll_fds.getCount(); i++)
	{
		dest[i] = poll_fds.getFds(i).fd;
	}
	return (dest);
}

std::vector<Connection>::iterator find_co_by_fd_it(__attribute__((unused))std::vector<Connection> all_co, int fd)
{
	std::vector<Connection>::iterator it = all_co.begin();
	std::vector<Connection>::iterator ite = all_co.end();

	for(; it!=ite; ++it)
	{
		if (fd == it->getFd())
			return it;
	}
	return ite;
}

int find_co_by_fd_pos(__attribute__((unused))std::vector<Connection> all_co, int fd)
{
	for(unsigned int i = 0; i < all_co.size(); ++i)
	{
		if (fd == all_co[i].getFd())
			return i;
	}
	return -1;
}

void	send_response(__attribute__((unused))Connection co) {}

void	launch_server(__attribute__((unused))ConfigFile config, Poll poll_fds)
{
	unsigned int *server_fd;
	int size_server_fd = poll_fds.getCount();

	std::vector<Connection>	all_co;
	server_fd = list_server_fd(poll_fds);
	while (true)
	{
		int	status = poll_fds.call_to_poll();
		if (status < 0)
			return (perror("poll"));
		else if (status == 0)
			continue;
		for(int i = 0; i < poll_fds.getCount(); i++)
		{
			if ((poll_fds.getFds(i).revents & POLLIN) == 1)
			{
				// new client requesting the server
				if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd, size_server_fd)) != -1)
					all_co.push_back(Connection(accept_new_connection(server_fd[status], &poll_fds)));
				// data to read from the client request
				else
				{
					std::string buff = read_recv_data(i, &poll_fds);
					if (buff == "error recv")
						return ; /*leaks pas encore gere*/
					else if (buff == "connection closed")
						continue ;
					all_co[find_co_by_fd_pos(all_co, poll_fds.getFds(i).fd)].getRq().appendRaw(buff);
					// function(buff, &poll_fds, i, config);
					//send(poll_fds.getFds(i).fd, rep.c_str(), rep.size(), 0);
				}
			}
			// responding
			else if (all_co.size() > 0 && (poll_fds.getFds(i).revents & POLLOUT) == 1) // pe direct checker de quelle connection on parle? avec un iterator (pour erase)
			{
				send_response(all_co[find_co_by_fd_pos(all_co, poll_fds.getFds(i).fd)]);
				all_co.erase(find_co_by_fd_it(all_co, poll_fds.getFds(i).fd));
			}	
		}
	}
	free(server_fd);
}

int	verif_host(ConfigFile config, int i)
{
	for (int y = 0; y < i; y++)
	{
		if (config.getServers()[y].getIp() == config.getServers()[i].getIp()
			&& config.getServers()[y].getPort() == config.getServers()[i].getPort())
			return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return std::cerr << "expected config file as single argument" << std::endl, 2;
	
	ConfigFile	config(av[1]);
	try{
		config.openReadFileToStr();
		config.readAllInfos();
	}
	catch(const std::exception& e){
		return std::cerr << e.what() << std::endl, 1;
	}
	Poll poll_fds;
	for (size_t i = 0; i < config.getServers().size(); i++)
	{
		if (verif_host(config, i) == 1)
			continue ;
		int fd = create_socket_server(config.getServers()[i].getPortSTR().c_str());
		if (fd == -1)
			return 1;
		try{
			poll_fds.add_to_poll(fd);
		}
		catch(const std::exception& e){
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	launch_server(config, poll_fds);
	return 0;
}
