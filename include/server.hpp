/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 15:29:50 by mmuesser          #+#    #+#             */
/*   Updated: 2024/08/07 15:34:50 by mmuesser         ###   ########.fr       */
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
#include <dirent.h>

/* Error default pages	*/
#define DEFAULT_400 "www/400_error_default.html"
#define DEFAULT_404 "www/404_error_default.html"
#define DEFAULT_405 "www/405_error_default.html"
#define DEFAULT_413 "www/413_error_default.html"
#define DEFAULT_500 "www/500_error_default.html"
#define DEFAULT_501 "www/501_error_default.html"
#define DEFAULT_505 "www/505_error_default.html"

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
void	rq_html(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc);

/*rq_dir.cpp*/
void	rq_dir(Response *rp, Request rq, ConfigFile config, Server serv, int index_loc, int index_serv);

/*exec_rq.cpp*/
int			check_body_size(Request rq);
Response	exec_rq(Request rq, ConfigFile config, int index_serv, int index_loc);
Response	exec_rq_error(Request rq, ConfigFile config, int code, int index_serv, int index_loc);

/*utils.cpp*/
int									check_file(std::string path, int mode);
int									find_location(const std::string path, Server serv);
int									find_location2(const std::string path, Server serv);
std::string							find_str_data(Server serv, int index_loc, std::string to_find);
std::map<std::string, std::string>	find_error_pages(Server serv, int index_loc);
std::vector<std::string>			find_vector_data(Server serv, int index_loc, std::string to_find);

#endif
