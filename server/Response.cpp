/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:54:11 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/13 16:09:15 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

Response::Response(void)
{}

Response::~Response(void)
{}

std::string Response::getLineState(void)
{return (_lineState):}

std::string Response::getHeader(void)
{return (_header);}

std::string Response::getBody(void)
{return (_body);}

void	Response::setLineState(std::string lineState)
{_lineState = lineState;}

void	Response::setHeader(std::string header)
{_header = header;}

void	Response::setBody(std::string body)
{_body = body;}