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

#include "../include/Response.hpp"

Response::Response(void)
{}

Response::~Response(void)
{}

std::string Response::getLineState(void) const
{return (_lineState);}

std::string Response::getHeader(void) const
{return (_header);}

std::string Response::getBody(void) const
{return (_body);}

void	Response::setLineState(std::string lineState)
{_lineState = lineState;}

void	Response::setHeader(std::string header)
{_header = header;}

void	Response::setBody(std::string body)
{_body = body;}