/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/18 18:43:03 by mmuesser         ###   ########.fr       */
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
#include "include/Poll.hpp"
#include "include/server.hpp"
#include <iostream>
#include <string>

void	launch_server(ConfigFile config, Poll poll_fds)
{
	int	server_fd[config.getServers().size()];
	for (int i = 0; i < config.getServers().size() - 1; ++i)
		server_fd[i] = poll_fds.getFds(i).fd;

	while (true)
	{
		int	status = poll_fds.call_to_poll();
		if (status < 0)
			return (perror("poll"));
		else if (status == 0)
			continue;
		for(int i = 0; i < poll_fds.getCount(); i++)
		{
			if ((poll_fds.getFds(i).revents && POLLIN) != 1) /*revents = event attendu pour la socket et POLLIN = event pour signal entrant*/
				continue ;
			if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd)) != -1)
				accept_new_connection(server_fd[status], poll_fds); /*si c'est une nouvelle connexion*/
			else
			{
				char *buff;
				buff = read_recv_data(i, poll_fds); /*si un client deja co envoie une requete*/
				if (!buff)
					return;
				function(buff, &poll_fds, i, config);
			}
		}
	}
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
		int fd = create_socket_server(config.getServers()[i].getPortSTR().c_str());
		if (fd == -1)
			return 1;
		try{
			poll_fds.add_to_poll(fd); // add_to_poll met pollin|pollout mais sur serverdocket juste besoin de pollin nsp si pb
		}
		catch(const std::exception& e){
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	launch_server(config, poll_fds);
}