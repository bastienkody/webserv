/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 15:29:50 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/30 16:17:56 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERs_HPP
#define SERVERs_HPP

#include "Poll.hpp"
#include "Response.hpp"
#include "../RequestParsing/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"

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

/*server.cpp*/
int 	create_socket_server(const char *port);
char*	read_recv_data(int i, Poll *poll_fds);
int		function(std::string buff, Poll *poll_fds, int i, ConfigFile config);
void	accept_new_connection(int server_fd, Poll *poll_fds);
int		check_serv_socket(int fd, unsigned int *serv_fds);

/*rq_html.cpp*/
void	get_html(Response *rp, Request rq);
void	post_html(Response *rp, Request rq);
void	delete_html(Response *rp, Request rq);
void	rq_html(Response *rp, Request rq);

/*exec_rq.cpp*/
int			check_body_size(Request rq);
Response	exec_rq(Request rq, ConfigFile config);

/*utils.cpp*/
char		**create_av(Request rq);
char		**create_env(Request rq);
int			check_file(Request rq, std::string dir, int mode);


#endif