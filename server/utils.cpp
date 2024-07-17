/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:48:42 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/17 19:06:26 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../RequestParsing/Request.hpp"
#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"


/*creer check dir -> return 0 si pas un dir*/

int	check_file(Request rq, std::string dir, int mode)
{
	std::string path = dir + rq.getRql().getUrl().getPath();
	if (access(path.c_str(), F_OK) == -1)
		return (1);
	if (mode == 1 && access(path.c_str(), R_OK) == -1)
		return (2);
	if (mode == 2 && access(path.c_str(), X_OK) == -1)
		return (2);
	return (0);
}

int find_location(std::string path, Server serv)
{
	size_t size = 0;
	int index = -1;
	for(size_t i = 0; i < serv.getLocations().size(); i++)
	{
		if (path == serv.getLocations()[i].getPath())
			return (i);
		if (serv.getLocations()[i].getIsPathAbsolute() == false)
		{
			for (size_t y = 0; y < serv.getLocations()[i].getPath().size(); y++)
			{
				if (serv.getLocations()[i].getPath()[y] != path[y] && y > size)
				{
					index = i;
					size = y;
				}
			}
		}
	}
	return (index);
}

/*template ?*/
// void	find_data(Server serv, void *data, std::string to_find)
// {
// 	for (size_t i = 0; i < serv.getLocations().size(); i++)
// 	{
// 		if ()
// 	}
// }