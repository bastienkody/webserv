/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:04:51 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/15 13:43:47 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include <cstring>
#include <sys/poll.h>

Poll::Poll(void)
{
	this->_count = 0;
}

Poll::~Poll(void)
{}

void	Poll::add_to_poll(int new_fd)
{
	if (this->_count >= 256) 
		throw std::out_of_range("Error: Not enough space in poll_fds");
	memset(&_fds[_count], 0, sizeof(struct pollfd));
	_fds[_count].fd = new_fd;
	_fds[_count].events = POLLIN | POLLOUT;
	this->_count += 1;
}

void	Poll::remove_to_poll(int i)
{
	if (_count > 1)
	{
		_fds[i] = _fds[_count - 1];
		memset(&_fds[_count - 1], 0, sizeof(struct pollfd));
	}
	else
		memset(&_fds[i], 0, sizeof(struct pollfd));
	_count--;
}

int	Poll::call_to_poll()
{
	return poll(_fds, _count, -1);
}

struct pollfd	Poll::getFds(int i)
{
	return (_fds[i]);
}


int	Poll::getCount(void) const
{
	return (this->_count);
}

void	Poll::setFds(int i, int new_fd, short event)
{
	this->_fds[i].fd = new_fd;
	this->_fds[i].fd = event;
	_count++;
}

void	Poll::setEvent(int i, short event)
{
	this->_fds[i].events = event;
}

void	Poll::setCount(int count)
{
	this->_count = count;
}

void	Poll::list_fd()
{
	int i = 0;
	while(i < _count)
	{
		std::cout<< "fds[" << i << "] : " << _fds[i].fd <<std::endl;
		i++;
	}
}
