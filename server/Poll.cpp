/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:04:51 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/23 18:38:10 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include <cstring>
#include <unistd.h>
#include <sys/poll.h>

Poll::Poll(void)
{
	this->_count = 0;
	this->_nb_serv = 0;
}

Poll::~Poll(void)
{
	//for (int i = 0; i < _count; ++i)
		//close(_fds[i].fd);
}

void	Poll::end_close_fd(void)
{
	for (int i = 0; i < _count; ++i)
		close(_fds[i].fd);
}


void	Poll::add_to_poll(int new_fd)
{
	if (this->_count >= 256)
		remove_to_poll(_nb_serv);
	memset(&_fds[_count], 0, sizeof(struct pollfd));
	_fds[_count].fd = new_fd;
	_fds[_count].events = POLLIN | POLLOUT;
	this->_count += 1;
}

void	Poll::remove_to_poll(int i)
{
	if (_fds[i].fd > 0)
		close(_fds[i].fd);
	if (_count > 1)
	{
		for(int y = i; y < _count - 1; y++)
		{
			_fds[y] = _fds[y + 1];
		}
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

struct pollfd	*Poll::getAllFds()
{
	return (_fds);
}

int	Poll::getCount(void) const
{
	return (this->_count);
}

int	Poll::getNBserv(void) const
{
	return (this->_nb_serv);
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

void	Poll::setNBserv(int nb_serv)
{
	this->_nb_serv = nb_serv;
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
