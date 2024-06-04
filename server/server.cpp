/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/04 16:34:47 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <sys/socket.h> /*socket, listen, bind, accept...*/
// #include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
// #include <string.h> /*memset*/
// #include <netdb.h> /*struc addrinfo*/
// #include <unistd.h> /*fcntl*/
// #include <fcntl.h> /*fcntl*/
// #include <stdio.h> /*perror*/
// #include <stdlib.h> /*exit*/
// #include <iostream> /*std::*/
// #include <poll.h> /*poll*/
#include "../include/Poll.hpp"

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
	- utiliser "signal" pour catch ctrl c et terminer le programme
	- mettre a la norme du projet (check fonction autorise et alternative pour fonctions C)
	- gerer leaks et fds
-------------------------*/


int create_socket_server(const char *port)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int server_fd;
	int status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	/*recup les infos necessaires dans res pour creer puis bind socket*/
	status = getaddrinfo(NULL, port, &hints, &res);
	if (status < 0)
		return (perror("getaddrinfo"), -1);
	server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (server_fd < 0)
		return (perror("socket"), freeaddrinfo(res), -1);
	fcntl(server_fd, F_SETFL, O_NONBLOCK); /*je vois pas encore de diff avec et sans*/
	status = bind(server_fd, res->ai_addr, res->ai_addrlen);
	if (status < 0)
		return (perror("bind"), freeaddrinfo(res), close(server_fd), -1);
	freeaddrinfo(res);
	status = listen(server_fd, 20);
	if (status < 0)
		return (perror("listen"), close(server_fd), -1);
	std::cout<< "[Server] New server socket created and listening to port " << port<<std::endl;
	return (server_fd);
}

int	read_recv_data(int i, Poll *poll_fds)
{
	int nb_bytes;
	char buff[10000];

	memset(&buff, 0, sizeof buff); /*vide le buffer a chaque fois (pas sur de le garder mais utile pour l'instant)*/
	nb_bytes = recv(poll_fds->getFds(i).fd, &buff, 10000, 0);
	if (nb_bytes < 0)
		return (perror("recv"), -1);
	else if (nb_bytes == 0)
		return (poll_fds->remove_to_poll(i), std::cout<< "[Server] Connexion with " << poll_fds->getFds(i).fd << " is closed."<<std::endl, 0);
	std::cout<< "[Client "<< poll_fds->getFds(i).fd<< "] " << buff;
	/*
		1	parsing build objet grace a buff
		2	appel a cgi (pas forcement directement mais en tout cas sans repasser par read_recv)
		3	si cgi a eu tout ce dont il avait besoin parsing modifie nb_bytes en x qui signifie qu'on peut quitter read_recv
			sinon parsing modifie nb_bytes en y et return objet
		4	nouvel appel a recv pour lire suite body
		5	appel a parsing avec objet != NULL donc pas besoin de rebuild juste append
		6	repeter a partir 2 autant de fois que necessaires
	*/
	return (0);
}

/*int	function(char *buff, int *nb_bytes, bool)
{
	parse buff et cree obj
	envoie a cgi
	cgi renvoie erreur manque info
	appel a read_recv
	append a obj
	envoie a cgi etc...
}*/

/*int	read_recv_data(int i, Poll *poll_fds)
{
	int nb_bytes;
	char buff[10000];
	int status;

	memset(&buff, 0, sizeof buff);
	nb_bytes = recv(poll_fds->getFds(i).fd, &buff, 10000, 0);
	if (nb_bytes < 0)
		return (perror("recv"), -1);
	else if (nb_bytes == 0)
		return (poll_fds->remove_to_poll(i), std::cout<< "[Server] Connexion with " << poll_fds->getFds(i).fd << " is closed."<<std::endl, 0);
	std::cout<< "[Client "<< poll_fds->getFds(i).fd<< "] " << buff;	
	return (0);
}*/

void	accept_new_connection(int server_fd, Poll *poll_fds)
{
	int client_fd;
	int status;

	client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0)
		return (perror("accept"), close(server_fd), exit(1));
	status = poll_fds->add_to_poll(client_fd);
	if (status < 0)
		return (close(server_fd), close(client_fd), exit(1));
	std::cout<< "[Server] New connexion with client fd : " << poll_fds->getFds(poll_fds->getCount() - 1).fd<<std::endl;
}

/*verifie quelle socket server a recu une nouvelle connexion*/
int	check_serv_socket(int fd, int *serv_fds)
{
	for (int i = 0; i < 3; i++)
	{
		if (fd == serv_fds[i])
			return (i);
	}
	return (-1);
}