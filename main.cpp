/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/05/26 15:24:45 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> /*socket, listen, bind, accept...*/
#include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
#include <string.h> /*memset*/
#include <netdb.h> /*struc addrinfo*/
#include <unistd.h> /*fcntl*/
#include <fcntl.h> /*fcntl*/
#include <stdio.h> /*perror*/
// #include <iostream> /*std::*/
// #include <poll.h> /*poll*/
#include "include/Poll.hpp"
#include <stdlib.h> /*exit*/

/*---------TO DO---------
- : pas encore fait/en cours
~ : fait mais pas sur que ce soit bien
+ : fait

	~ finir mise en place serv test
		~ creer class poll pour faciliter manip (notamment l'update du pollfd) puis finir implementation
		- creer class socket (je sais pas encore comment je vais m'en servir)
		~ passer socket en non bloquant
		+ POUVOIR ECOUTER SUR PLUSIEURS PORTS (creer socket server pour chaque port ?
			socketpair pas utilise parce qu'utile pour 2 ports mais a partir de 3 revient au meme que utiliser socket
	- commencer parsing une fois serv fait (Bastien ou moi qui fait ?)
	- utiliser "signal" pour catch ctrl c et terminer le programme
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

	/*recup les infos necessaires pour creer puis bind socket*/
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
	return (0);
}

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

/*verifie quel socket server a recu une nouvelle connexion*/
int	check_serv_socket(int fd, int *serv_fds)
{
	for (int i = 0; i < 3; i++)
	{
		if (fd == serv_fds[i])
			return (i);
	}
	return (-1);
}

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
			}
		}
	}
	return (0);
}


/*Probablement bientot supp*/

// int	main(void)
// {
// 	struct addrinfo hints;
// 	struct addrinfo *res;
// 	int server_fd; /*fd du socket bind sur mon serveur*/
// 	int client_fd; /*fd du client qui se connect au serv via server_fd*/
// 	int status; /*check retour diff fonctions*/

// 	memset(&hints, 0, sizeof hints);
// 	hints.ai_family = AF_INET;
// 	hints.ai_socktype = SOCK_STREAM;
// 	hints.ai_flags = AI_PASSIVE;
	
// 	status = getaddrinfo(NULL, PORT, &hints, &res);
// 	if (status != 0)
// 	{
// 		perror("getaddrinfo");
// 		return (std::cout<< "Error getaddrinfo"<<std::endl, 0);
// 	}
// 	server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
// 	if (server_fd == -1)
// 		return (freeaddrinfo(res), std::cout<< "Error socket"<<std::endl, 0);
// 	//server_fd = socket(PF_INET, SOCK_STREAM, TCP);

// 	status = bind(server_fd, res->ai_addr, res->ai_addrlen);
// 	if (status == -1)
// 	{
// 		perror("bind");
// 		return (freeaddrinfo(res), close(server_fd), std::cout<< "Error bind"<<std::endl, 0);
// 	}

// 	status = listen(server_fd, 20);
// 	if (status == -1)
// 	{
// 		perror("listen");
// 		return (freeaddrinfo(res), close(server_fd), std::cout<< "Error listen"<<std::endl, 0);
// 	}
// 	std::cout<< "[Server] Server is listening on port " << PORT <<std::endl;
	
// 	client_fd = accept(server_fd, NULL, 0);
// 	if (client_fd == -1)
// 	{
// 		perror("accept");
// 		return (freeaddrinfo(res), close(server_fd), std::cout<< "Error accept"<<std::endl, 0);
// 	}
// 	std::cout<< "[Server] New client socket : " << client_fd <<std::endl;
	
// 	ssize_t nb_bytes;
// 	char buff[10000];
// 	while (1)
// 	{
// 		memset(&buff, 0, 10000);
// 		nb_bytes = recv(client_fd, &buff, 10000, 0);
// 		if (nb_bytes == 0)
// 			break ;
// 		else if (nb_bytes == -1)
// 			return (freeaddrinfo(res), close(client_fd), close(server_fd), std::cout<< "Error recv"<<std::endl, 0);
// 		std::cout<< "[Client] " << buff;
// 	}
// 	std::cout<< "[Server] Connexion is closed."<<std::endl;
// 	freeaddrinfo(res);
// 	close(server_fd);
// 	close(client_fd);
// 	return (0);
// }