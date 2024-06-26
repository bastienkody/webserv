/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:48:03 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/14 17:36:24 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>

class Response
{
	public:

		Response(void);
		~Response(void);

		std::string getLineState() const;
		std::string getHeader() const;
		std::string getBody() const;

		void	setLineState(std::string lineState);
		void	setHeader(std::string header);
		void	setBody(std::string body);

	private:

		std::string _lineState;
		std::string _header;
		std::string _body;
};

#endif