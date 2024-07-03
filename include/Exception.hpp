/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:48:39 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/28 18:28:46 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <iostream>

class Exception : public std::exception
{
	public:
		Exception(void) throw() {}
		~Exception(void) throw() {}
		Exception(int mode) throw() : _mode(mode) {}
		char const *what() const throw()
		{
			if (_mode == 1)
				return (char *) "Error check_file";
			else if (_mode == 2)
				return (char *) "Error pipe";
			else if (_mode == 3)
				return (char *) "Error fork"; 
			else 
				return (char *) "Error Method no supported";
		}
	private:
		int _mode;
};

#endif
