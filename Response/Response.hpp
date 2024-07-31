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
#include <sstream>
#include <ctime>
#include "../StatusCode/StatusCode.hpp"
#include "../RequestParsing/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../ParserUtils/ParserUtils.hpp"

class Response
{
	public:

		Response(void);
		~Response(void);

		std::string const & getLineState() const;
		std::string const & getHeader() const;
		std::string const & getBody() const;

		std::string getWholeResponse() const;

		void	setLineState(int code);
		void	setHeader(Request rq, ConfigFile config, int serv_nb, int loc_nb);
		void	setBody(std::string body, std::string type);

		// utils header body
		void	setContentLength(unsigned int body_size);
		void	setContentType(std::string type);
		//	utils header
		std::string	hcreateTimeStr() const;
		std::string	hcreateConnection(const Request & rq) const;
		std::string	hcreateServer() const;
		std::string	hcreateAllowMethods(ConfigFile config, int serv_nb, int loc_nb) const;

	private:

		std::string _lineState;
		std::string _header;
		std::string _body;
};

#endif
