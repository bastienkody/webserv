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
	return _lineState + "\r\n" + _header + "\r\n\r\n" + _body; 
}

//	setters
void	Response::setLineState(int code)
{
	StatusCode		sc;
	std::stringstream	sstr;

	sstr << code;
	_lineState = "HTTP/1.1 " + sstr.str() + " " + sc.getPhrase(code);
}

void	Response::setHeader(__attribute__((unused))Request rq, __attribute__((unused))ConfigFile config)
{
	_header += hcreateTimeStr() + "\n"; // date
	_header += hcreateServer() + "\n"; // server
	// connection
	std::string connection = hcreateConnection(rq);
	if (connection.size())
		_header += connection + "\n";
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
std::string	Response::hcreateTimeStr() const
{
	// date: Thu, 04 Jul 2024 12:24:32 GMT
	std::time_t s_epoch = std::time(0);
	std::string res("Date: ");
	char	timeSTR[200];

	std::strftime(timeSTR, sizeof(timeSTR), "%a, %d %b %Y %T %Z", std::localtime(&s_epoch));
	res += timeSTR;

	return res;
}

std::string	Response::hcreateConnection(const Request & rq) const
{
	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();

	for (; it!=ite; ++it)
	{
		if (ParserUtils::compCaseInsensitive(it->first, "Connection") && ParserUtils::compCaseInsensitive(it->second, "Keep-Alive"))
			return "Connection: Keep-Alive";
		if (ParserUtils::compCaseInsensitive(it->first, "Connection") && ParserUtils::compCaseInsensitive(it->second, "Close"))
			return "Connection: Close";
	}
	return "";
}


std::string	Response::hcreateServer() const
{
	return "server: webserv 1.0";
}

std::string	Response::hcreateAllowMethods(__attribute__((unused))ConfigFile config) const
{
	std::string res("allow: ");

	return res;

}


