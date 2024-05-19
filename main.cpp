/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/05/19 17:17:01 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> /*socket, listen, bind, accept...*/
#include <arpa/inet.h> /*inet_pton et inet_ntop*/
#include <string.h> /*memset*/
#include <netdb.h> /*struc addrinfo*/
#include <iostream> /*std::*/

/*dans un premier temps creer socket
  pour ca je dois utiliser getaddrinfo pour obtenir info pour creer socket et pour utiliser bind and co*/

int	main(void)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int socket_fd; /*fd du socket bind sur mon serveur*/
	int client_fd; /*fd du client qui se connect au serv via socket_fd*/
	int status; /*check retour diff fonctions*/

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	status = getaddrinfo(NULL, "8080", &hints, &res);
	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	//socket_fd = socket(PF_INET, SOCK_STREAM, TCP);

	status = bind(socket_fd, res->ai_addr, res->ai_addrlen);

	status = listen(socket_fd, 20);
	
	client_fd = accept(socket_fd, NULL, 0);
	
	std::cout<< "Nouvelle connection recue !"<<std::endl;
	return (0);
}