/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:54:11 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/14 21:45:36 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response.hpp"

//	default
Response::Response(void) {}
Response::~Response(void){}

//	getters
std::string const & Response::getLineState(void) const {return (_lineState);}
std::string const & Response::getHeader(void) const {return (_header);}
std::string const & Response::getBody(void) const {return (_body);}

std::string Response::getWholeResponse(void) const
{
	// attention si final \n in header ajouter que un seul ici avant body
	return _lineState + "\r\n" + _header + "\n\n" + _body; 
}

//	setters
void	Response::setLineState(int code)
{
	StatusCode		sc;
	std::stringstream	sstr;

	sstr << code;
	_lineState = "HTTP/1.1 " + sstr.str() + " " + sc.getPhrase(code);
}

void	Response::setHeader(__attribute__((unused))Request rq, ConfigFile config)
{
	_header += createTimeStr() + "\n"; // date
	_header += "server: webserv 1.0\n"; // server
	// connection
	// etag
	// host ?
	// allow methods (on le et toujours comme ca on est tranquille)
	// si body (at least get and post, )
		// content type
		// content lenght
}

void	Response::setBody(std::string body)
{_body = body;}

/*
	utils header
*/
std::string	Response::createTimeStr() const
{
	// date: Thu, 04 Jul 2024 12:24:32 GMT
	std::time_t s_epoch = std::time(0);
	std::string res("date: ");
	char	timeSTR[200];

	std::strftime(timeSTR, sizeof(timeSTR), "%a, %d %b %Y %T %Z", std::localtime(&s_epoch));
	res += timeSTR;

	return res;
}

std::string	Response::createAllowMethods(ConfigFile config) const
{
	std::string res("allow: ");



}


