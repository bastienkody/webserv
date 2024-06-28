/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:37:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/28 18:35:34 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef POLL_HPP
#define POLL_HPP

#include <poll.h>
#include <iostream>

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