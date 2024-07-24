/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_dir.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:03:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/23 13:05:32 by mmuesser         ###   ########.fr       */
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

std::string read_index(std::string index)
{
	return (index);
}

void	rq_dir(Response *rp, Request rq, std::string path, Server serv, int index_location)
{
	bool auto_index = check_auto_index(serv, index_location);

	if (serv.getLocations()[index_location].getIndex().size() != 0)
		return (rp->setBody(read_index(serv.getLocations()[index_location].getIndex())));
	else if (serv.getIndex().size() != 0)
		return (rp->setBody(read_index(serv.getIndex())));
	// else if (auto_index == true)
	// 	/*create index*/
	// else
	// 	/*403*/
	(void) auto_index;
	(void) rp;
	(void) rq;
	(void) path;

}