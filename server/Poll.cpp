/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:04:51 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/30 16:18:57 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"

Poll::Poll(void)
{
	this->_count = 0;
}

Poll::~Poll(void)
{}

void	Poll::add_to_poll(int new_fd)
{
	if (this->_count >= 256) // 255 nan vu qu'on commence a 0 ?? renommer en index vs count ??
		throw std::out_of_range("Error: Not enough space in poll_fds");
	_fds[_count].fd = new_fd;
	_fds[_count].events = POLLOUT | POLLIN;
	this->_count += 1;
}

void	Poll::remove_to_poll(int i)
{
	_fds[i] = _fds[_count - 1]; // count commence pas a 0? // si mais mon dernier fd est a l'index count - 1
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
