/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Archi.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 15:29:50 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/25 16:23:27 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARCHI_HPP
#define ARCHI_HPP

#include "Poll.hpp"
#include "Response.hpp"
#include "Request.hpp"

#include <poll.h>
#include <iostream>
#include <sys/socket.h> /*socket, listen, bind, accept...*/
#include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
#include <string.h> /*memset*/
#include <netdb.h> /*struc addrinfo*/
#include <unistd.h> /*fcntl*/
#include <fcntl.h> /*fcntl*/
#include <stdio.h> /*perror*/
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

/*server.cpp*/
int 	create_socket_server(const char *port);
int		read_recv_data(int i, Poll *poll_fds);
void	accept_new_connection(int server_fd, Poll *poll_fds);
int		check_serv_socket(int fd, int *serv_fds);

// /*rq_html.cpp*/
// void	get_html(Response *rp, Request rq);
// void	post_html(Response *rp, Request rq);
// void	delete_html(Response *rp, Request rq);
// void	rq_html(Response *rp, Request rq);

// /*exec_rq.cpp*/
// int			check_body_size(Request rq);
// Response	exec_rq(Request rq);

// /*CGI.cpp*/
// const char	**create_av(const char *name, const char *arg);
// char 		**create_env(Request rq);
// std::string create_path_name(Request rq);
// void		exec_son(Request rq, int *pipe_fd);
// char		*exec_father(Request rq, int *pipe_fd);
// int 		cgi(Response *rp, Request rq);

// /*utils.cpp*/
// int	check_file(Request rq, std::string dir, int mode);


#endif