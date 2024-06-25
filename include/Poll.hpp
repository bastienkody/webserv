/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:37:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/14 17:33:16 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef POLL_HPP
#define POLL_HPP


#include "server.hpp"
#include <poll.h>
// #include <iostream>
// #include <sys/socket.h> /*socket, listen, bind, accept...*/
// #include <arpa/inet.h> /*inet_pton et inet_ntop  (transforme ip en chaine de caracteres et inversement)*/
// #include <string.h> /*memset*/
// #include <netdb.h> /*struc addrinfo*/
// #include <unistd.h> /*fcntl*/
// #include <fcntl.h> /*fcntl*/
// #include <stdio.h> /*perror*/
// #include <stdlib.h> /*exit*/


class	Poll
{
	public :
		Poll(void);
		~Poll(void);
		// Poll & operator=(const Poll &obj);
		struct pollfd	getFds(int i) const;
		int				getCount(void) const;
		void			setFds(int i, int new_fd, short event);
		void			setCount(int count);
		void			add_to_poll(int new_fd); // throw
		void			remove_to_poll(int i);
		int				call_to_poll(void);

	private :
		struct pollfd	_fds[256];
		int				_count;
};

#endif