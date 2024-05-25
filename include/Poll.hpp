/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:37:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/05/25 17:02:13 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <iostream>

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