/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 15:29:50 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/24 17:28:44 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "Poll.hpp"
#include "../Response/Response.hpp"
#include "../RequestParsing/Request.hpp"
#include "../RequestChecking/RequestChecking.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../ConfigFile/Server.hpp"

#include <iostream>
#include <sys/socket.h> /*socket, listen, bind, accept...*/
#include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
#include <string.h> /*memset*/
#include <netdb.h> /*struc addrinfo*/
#include <unistd.h> /*fcntl*/
#include <fcntl.h> /*fcntl*/
#include <stdio.h> /*perror*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <map>

/*server.cpp*/
int 	create_socket_server(const char *port);
std::string	read_recv_data(int i, Poll *poll_fds);
int	send_response(struct client &co, __attribute__((unused))ConfigFile config);
int	accept_new_connection(int server_fd, Poll *poll_fds);
int		check_serv_socket(int fd, unsigned int *serv_fds, int size);

/*rq_html.cpp*/
void	get_html(Response *rp, Request rq);
void	post_html(Response *rp, Request rq);
void	delete_html(Response *rp, Request rq);
void	rq_html(Response *rp, Request rq);

/*rq_dir.cpp*/
void	rq_dir(Response *rp, Request rq, std::string path, Server serv, int index_location);

/*exec_rq.cpp*/
int			check_body_size(Request rq);
Response	exec_rq(Request rq, ConfigFile config, int index_serv);
Response	exec_rq_error(Request rq, ConfigFile config, int code);

/*utils.cpp*/
int									check_file(Request rq, std::string dir, int mode);
int									find_location(const std::string path, Server serv);
std::string							find_str_data(Server serv, int index_location, std::string to_find);
std::map<std::string, std::string>	find_error_pages(Server serv, int index_location);
std::vector<std::string>			find_vector_data(Server serv, int index_location, std::string to_find);

#endif
