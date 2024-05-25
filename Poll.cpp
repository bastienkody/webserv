/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:04:51 by mmuesser          #+#    #+#             */
/*   Updated: 2024/05/25 15:39:03 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Poll.hpp"

Poll::Poll(void)
{
	this->_count = 0;
}

Poll::~Poll(void)
{}

int	Poll::add_to_poll(int new_fd)
{
	if (this->_count >= 256)
		return (std::cout<<"Error: Not enough space in \"poll_fds\""<<std::endl, -1);
	_fds[_count].fd = new_fd;
	_fds[_count].events = POLLIN;
	this->_count++;
	return (0);
}

void	Poll::remove_to_poll(int i)
{
	_fds[i] = _fds[_count - 1];
	_count--;
}

int	Poll::wait()
{
	int status = poll(_fds, _count, -1);
	return (status);
}

struct pollfd	Poll::getFds(int i) const
{
	return (_fds[i]);
}

// struct pollfd Poll::getAllFds(void) const
// {
// 	return (this->_fds);
// }

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