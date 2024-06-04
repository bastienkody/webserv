/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:37:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/04 15:04:56 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <iostream>
#include <sys/socket.h> /*socket, listen, bind, accept...*/
#include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
#include <string.h> /*memset*/
#include <netdb.h> /*struc addrinfo*/
#include <unistd.h> /*fcntl*/
#include <fcntl.h> /*fcntl*/
#include <stdio.h> /*perror*/
#include <stdlib.h> /*exit*/


class	Poll
{
	public :
		Poll(void);
		~Poll(void);
		struct pollfd	getFds(int i) const;
		int				getCount(void) const;
		void			setFds(int i, int new_fd, short event);
		void			setCount(int count);
		int 			add_to_poll(int new_fd);
		void			remove_to_poll(int i);
		int				wait(void); /*trouver meilleur nom ?*/

	private :
		struct pollfd	_fds[256];
		int				_count;
};

int create_socket_server(const char *port);
int	read_recv_data(int i, Poll *poll_fds);
void	accept_new_connection(int server_fd, Poll *poll_fds);
int	check_serv_socket(int fd, int *serv_fds);