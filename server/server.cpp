/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/18 18:41:33 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "ConfigFile/ConfigFile.hpp"


// passer le add_to_poll ici ? utiliser les excptions ?
int create_socket_server(const char *port)
{
	struct addrinfo hints, *res;
	int server_fd, status, flag_yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// addr data
	if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0)
		return (perror("getaddrinfo"), -1);
	// create socket
	if ((server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
		return (perror("socket"), freeaddrinfo(res), -1);
	// set extra behaviour
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag_yes, sizeof(int))!= 0)
		return (perror("setsockopt:"), -1);
	// attach socket to ip/port
	if ((status = bind(server_fd, res->ai_addr, res->ai_addrlen)) < 0)
		return (perror("bind"), freeaddrinfo(res), close(server_fd), -1);
	freeaddrinfo(res);
	// make it listen to client
	if ((status = listen(server_fd, 20))< 0)
		return (perror("listen"), close(server_fd), -1);
	std::cout<< "[Server] New server socket created and listening to port " << port <<std::endl;
	return (server_fd);
}

char	*read_recv_data(int i, Poll poll_fds)
{
	int nb_bytes;
	char buff[10000];

	memset(&buff, 0, sizeof buff); /*vide le buffer a chaque fois (pas sur de le garder mais utile pour l'instant)*/
	nb_bytes = recv(poll_fds.getFds(i).fd, &buff, 10000, 0);
	if (nb_bytes < 0)
		return (perror("recv"), (char*)NULL);
	else if (nb_bytes == 0)
		return (poll_fds.remove_to_poll(i), std::cout<< "[Server] Connexion with " << poll_fds.getFds(i).fd << " is closed."<<std::endl, (char*)NULL);
	std::cout<< "[Client "<< poll_fds.getFds(i).fd<< "] " << buff;
	return (buff);
}

int	function(std::string buff, Poll poll_fds, int i, ConfigFile config)
{
	Request rq(buff);

	while (check_body_size(rq) == -1)
	{
		char *tmp = read_recv_data(i, poll_fds);
		if (!tmp)
			return (std::cerr<< "Error read_recv_data"<<std::endl, -1);
		rq.appendBody(tmp);
	}
	//exec_rq
	return (0);
}

/*	attention a exit nsp si on appelle bien les destructeurs cpp	*/
void	accept_new_connection(int server_fd, Poll poll_fds)
{
	int client_fd;

	client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0)
		return (perror("accept"), close(server_fd), exit(1));
	if (poll_fds.getCount() > 255)
		return (close(server_fd), close(client_fd), exit(1));
	poll_fds.add_to_poll(client_fd);
	std::cout<< "[Server] New connexion with client fd : " << poll_fds.getFds(poll_fds.getCount() - 1).fd<<std::endl;
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