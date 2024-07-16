/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/16 17:31:24 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ConfigFile/ConfigFile.hpp"
#include "../include/server.hpp"
#include "../Clients/Clients.hpp"

static std::string rep("HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 5\nContent-Type: text/html\nConnection: Keep-alive\n\npipi\n");

int create_socket_server(const char *port)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int server_fd;
	int flag_true = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, &res) < 0)
		return (perror("getaddrinfo"), -1);
	// create the server socket
	if ( (server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
		return (perror("socket"), freeaddrinfo(res), -1);
	// set options
	fcntl(server_fd, F_SETFL, O_NONBLOCK); /*je vois pas encore de diff avec et sans*/
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &flag_true, sizeof(flag_true)) != 0)
		return (perror("setsockopt"), -1);
	// make it listen on port
	if (bind(server_fd, res->ai_addr, res->ai_addrlen) < 0)
		return (perror("bind"), freeaddrinfo(res), close(server_fd), -1);
	freeaddrinfo(res);
	if (listen(server_fd, 20) < 0)
		return (perror("listen"), close(server_fd), -1);
	std::cout<< "[Server] New server socket created and listening to port " << port<<std::endl;
	return (server_fd);
}

std::string read_recv_data(int i, Poll *poll_fds)
{
	int nb_bytes;
	char buff[1024];
	
	memset(&buff, 0, sizeof(buff));
	nb_bytes = recv(poll_fds->getFds(i).fd, &buff, 1023, 0);
	// si nb_bytes == 0 ou < 0 => virer le client sans lui repondre
	if (nb_bytes < 0)
		return (perror("recv"), "error recv");
	else if (nb_bytes == 0)
		return (poll_fds->remove_to_poll(i), std::cout<< "[Server] Connexion with " << poll_fds->getFds(i).fd << " is closed."<<std::endl, "connection closed");
	std::cout<< "[Client "<< poll_fds->getFds(i).fd<< "] " << std::endl;
	return (buff);
}

int	send_response(struct client &co, __attribute__((unused))ConfigFile config)
{
	std::cout << "send response to fd " << co.fd << std::endl;
	co.rq.parse();
	co.rq.print();


	
	return send(co.fd, rep.c_str(), rep.size(), 0);// si erreur de send => virer le client sans re essayer de lui repondre.
}


// int	function(__attribute__((unused))std::string buff, Poll *poll_fds, int i, ConfigFile config)
// {
// 	Request rq;
// 	int	code;

// 	//First check syntax, verb, version, host header present and headerfield syntax
// 	if ((code = RequestChecking::CheckBasics(rq)) != 0)
// 		return (exec_rq_error(rq, config, code), 0);
// 	//If post method, check if chunked (getMaxbodysize to be corrected)
// 	if (rq.getRql().getVerb().compare("POST") == 0 && (code = RequestChecking::CheckRequiredHeaderPOST(rq, config.getMaxBodySize())) != 1)
// 	{
// 		if (code == 413 || code == 0) // maybe 0 must become a 400 
// 			return (exec_rq_error(rq, config, code), 0);
// 		if (code == 2)
// 			rq.unchunk(poll_fds->getFds(i).fd);
// 	}
// 	exec_rq(rq, config);
// 	std::cout << "in function after rq" << std::endl;
// 	return (0);
// }

/*	attention a exit !! ca free bien? ca pose peut poser pb pour co. pe passer par des exceptions?	*/
int	accept_new_connection(int server_fd, Poll *poll_fds)
{
	int client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0)
		return (perror("accept"), close(server_fd), exit(1), 0);
	if (poll_fds->getCount() > 255)
		return (close(server_fd), close(client_fd), exit(1), 0);
	poll_fds->add_to_poll(client_fd);
	std::cout<< "[Server] New connexion with client fd : " << poll_fds->getFds(poll_fds->getCount() - 1).fd<<std::endl;
	return client_fd;
}

/*verifie quelle socket server a recu une nouvelle connexion*/
int	check_serv_socket(int fd, unsigned int *serv_fds, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if ((unsigned int) fd == serv_fds[i])
			return (i);
	}
	return (-1);
}
