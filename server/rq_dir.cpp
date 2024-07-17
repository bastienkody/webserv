/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_dir.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:03:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/17 15:11:52 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"

bool	check_auto_index(Server serv, int index_location)
{
	if (serv.getLocations()[index_location].getAutoIndex() == "on")
		return (true);
	else if (serv.getLocations()[index_location].getAutoIndex() == "off")
		return (false);
	if (serv.getAutoIndex() == "on")
		return (true);
	else
		return (false);
}

void	rq_dir(Response *rp, Request rq, std::string path, Server serv, int index_location)
{
	bool auto_index = check_auto_index(serv, index_location);

	(void) rp;
	(void) rq;
	(void) path;
	(void) auto_index;
	/*si index present (dans location ou server) renvoie index
	  si index non present et que auto_index == true cree index avec readdir
	  si index non present et que auto_index == false renvoie 403*/
}