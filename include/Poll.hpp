/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:37:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/30 16:06:28 by mmuesser         ###   ########.fr       */
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
		struct pollfd	getFds(int i) ;
		struct pollfd	*getAllFds();
		int			getCount(void) const;
		void			setFds(int i, int new_fd, short event);
		void			setEvent(int i, short event);
		void			setCount(int count);
		void			add_to_poll(int new_fd); // throw
		void			remove_to_poll(int i);
		int			call_to_poll(void);
		void			end_close_fd(void);

		void			list_fd();

	private :
		struct pollfd	_fds[256];
		int				_count;
};

#endif
