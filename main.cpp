/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/05/25 15:45:03 by mmuesser         ###   ########.fr       */
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

#define PORT "8080"

/*dans un premier temps creer socket
  pour ca je dois utiliser getaddrinfo pour obtenir info pour creer socket 
  et pour utiliser bind and co grace au resultat de getaddrinfo
  utiliser shutdown pour socket ?*/


/*---------TO DO---------
	- finir mise en place serv test
		- creer class poll pour faciliter manip (notamment l'update du pollfd) puis finir implementation
		- creer class socket (je sais pas encore comment je vais m'en servir)
		- passer socket en non bloquant (fait ?)
	- commencer parsing une fois serv fait
	- utiliser "signal" pour catch ctrl c et terminer le programme*/


int create_socket_server(void)
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
	status = getaddrinfo(NULL, PORT, &hints, &res);
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
	std::cout<< "[Server] New server socket created and listening to port " << PORT<<std::endl;
	return (server_fd);
}

int	read_recv_data(int i, Poll *poll_fds)
{
	int nb_bytes;
	char buff[10000];

	memset(&buff, 0, sizeof buff);
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


int main(void)
{
	int server_fd;
	int status;
	Poll poll_fds;

	server_fd = create_socket_server();
	if (server_fd == -1)
		return (0);
	status = poll_fds.add_to_poll(server_fd);

	while (1)
	{
		status = poll_fds.wait();
		if (status < 0)
			return (perror("poll"), close(server_fd), 0);
		else if (status == 0)
		{
			std::cout<< "[Server] Server is waiting..."<<std::endl;
			continue;
		}
		for(int i = 0; i < poll_fds.getCount(); i++)
		{
			if ((poll_fds.getFds(i).revents && POLLIN) != 1)
				continue ;

			if (poll_fds.getFds(i).fd == server_fd)
			{
				accept_new_connection(server_fd, &poll_fds);
			}
			else
			{
				status = read_recv_data(i, &poll_fds);
				if (status == -1)
					return (0);

			}
		}
	}
	
	return (0);
}




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