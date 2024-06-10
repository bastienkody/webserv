/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/10 14:16:55 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*---------TO DO---------
- : pas encore fait/en cours
~ : fait mais pas sur que ce soit bien
+ : fait

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

#include "include/Poll.hpp"

int main(void)
{
	/*bcp de hard code mais je le changerai quand le parsing du fichier de config sera fait 
	et qu'on aura les ports + leur nombre*/
	int server_fd[3];
	int status;
	Poll poll_fds; /*contient la liste des fds que poll va surveiller*/
	std::string ports[3] = {"8080", "8081", "8082"};

	for (int i = 0; i < 3; i++)
	{
		server_fd[i] = create_socket_server(ports[i].c_str());
		if (server_fd[i] == -1)
			return (0);
		status = poll_fds.add_to_poll(server_fd[i]);
		if (status < 0)
			return (0);
	}

	/*boucle principale du serv qui passe par poll a chaque tour pour les potentiels ecritures/lectures*/
	while (1)
	{
		status = poll_fds.wait(); /*appel a poll*/
		if (status < 0)
			return (perror("poll"), 0);
		else if (status == 0)
		{
			// std::cout<< "[Server] Server is waiting..."<<std::endl;
			continue;
		}
		for(int i = 0; i < poll_fds.getCount(); i++)
		{
			if ((poll_fds.getFds(i).revents && POLLIN) != 1) /*revents = event attendu pour la socket et POLLIN = event pour signal entrant*/
				continue ;

			if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd)) != -1)
			{
				accept_new_connection(server_fd[status], &poll_fds); /*si c'est une nouvelle connexion*/
			}
			else
			{
				status = read_recv_data(i, &poll_fds); /*si un client deja co envoie une requete*/
				if (status == -1)
					return (0);
				/*function*/
			}
		}
	}
	return (0);
}