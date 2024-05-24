/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/05/24 17:50:02 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> /*socket, listen, bind, accept...*/
#include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
#include <string.h> /*memset*/
#include <netdb.h> /*struc addrinfo*/
#include <iostream> /*std::*/
#include <unistd.h> /*fcntl*/
#include <fcntl.h> /*fcntl*/
#include <stdio.h> /*perror*/
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
		- passer socket en non bloquant
	- commencer parsing une fois serv fait*/


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

	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status < 0)
		return (perror("getaddrinfo"), -1);
	server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (server_fd < 0)
		return (perror("socket"), freeaddrinfo(res), -1);
	status = bind(server_fd, res->ai_addr, res->ai_addrlen);
	if (status < 0)
		return (perror("socket"), freeaddrinfo(res), close(server_fd), -1);
	freeaddrinfo(res);
	std::cout<< "[Server] New server socket created and bound to port " << PORT<<std::endl;
	return (server_fd);
}

// int	add_to_poll(int client_fd, struct pollfd *poll_fds, int *poll_count, int poll_size)
// {
// 	if (*poll_count >= poll_size)
// 		return (std::cout<<"Error: Not enough space in \"poll_fds\""<<std::endl, -1);
// 	(void) poll_size;
// 	// int i = -1;
// 	// while (++i < poll_size)
// 	// {
// 	// 	if ((*pol_fds)[i].fd == 0)
// 	// 		break ;
// 	// }
// 	// poll_fds[0]->fd = 10;
// 	poll_fds[*poll_count - 1].fd = client_fd;
// 	poll_fds[*poll_count - 1].events = POLLIN;
// 	*poll_count += 1;
// 	return (0);
// }

// void remove_to_poll(int i, struct pollfd *poll_fds, int *poll_count)
// {
// 	poll_fds[i] = poll_fds[*poll_count - 1];
// 	*poll_count -= 1;
// }

void	read_recv_data(int i, Poll *poll_fds)
{
	int nb_bytes;
	char buff[10000];

	nb_bytes = recv(poll_fds->getFds(i).fd, &buff, 10000, 0);
	if (nb_bytes < 0)
		return (perror("recv"), exit(1));
	else if (nb_bytes == 0)
		return (poll_fds->remove_to_poll(i), std::cout<< "[Server] Connexion with " << poll_fds->getFds(i).fd << "is closed."<<std::endl, exit(1));
	std::cout<< "[Client] " << buff<<std::endl;
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
	
	status = listen(server_fd, 20);
	if (status < 0)
		return (perror("listen"), close(server_fd), 0);
	status = poll_fds.add_to_poll(server_fd);

	while (1)
	{
		status = poll_fds.wait();
		std::cout<< "status : " << status<<std::endl;
		if (status < 0)
			return (perror("poll"), close(server_fd), 0);
		else if (status == 0)
		{
			std::cout<< "[Server] Server is waiting..."<<std::endl;
			continue;
		}
		// std::cout<< "count : " << poll_fds.getCount()<<std::endl;
		for(int i = 0; i < poll_fds.getCount(); i++)
		{
			if ((poll_fds.getFds(i).revents && POLLIN) != 1)
				continue ;

			if (poll_fds.getFds(i).fd == server_fd)
			{
				std::cout<< "test 1"<<std::endl;
				accept_new_connection(server_fd, &poll_fds);
			}
			// else
			// 	read_recv_data(i, &poll_fds);
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