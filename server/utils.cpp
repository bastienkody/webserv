/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:48:42 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/24 14:31:41 by mmuesser         ###   ########.fr       */
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

/*A modifier ?*/
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
template <typename T>
T	find_data(Server serv, int index_location, std::string to_find)
{
	std::string lst_data[9] = {"root", "index", "auto_index", "body_size", "redirection", "error_pages", "allow_methods", "cgi_ext", "cgi_pathes"};
	for(size_t i = 0; i < lst_data->size(); i++)
	{
		if (to_find == lst_data[i])
		{
			switch (i)
			{
				case 0: 
					if (serv.getLocations()[index_location].getRoot().size() != 0)
						return (serv.getLocations()[index_location].getRoot());
					else if (serv.getRoot().size() != 0)
						return (serv.getRoot());
					break ;
				case 1:
					if (serv.getLocations()[index_location].getIndex().size() != 0)
						return (serv.getLocations()[index_location].getIndex());
					else if (serv.getIndex().size() != 0)
						return (serv.getIndex());
					break ;
				case 2:
					if (serv.getLocations()[index_location].getAutoIndex().size() != 0)
						return (serv.getLocations()[index_location].getAutoIndex());
					else if (serv.getAutoIndex().size() != 0)
						return (serv.getAutoIndex());
					break ;
				case 3:
					if (serv.getLocations()[index_location].getMaxBodySize().size() != 0)
						return (serv.getLocations()[index_location].getMaxBodySize());
					else if (serv.getMaxBodySize().size() != 0)
						return (serv.getMaxBodySize());
					break ;
				case 4:
					if (serv.getLocations()[index_location].getRedirection().size() != 0)
						return (serv.getLocations()[index_location].getRedirection());
					else if (serv.getRedirection().size() != 0)
						return (serv.getRedirection());
					break ;
				case 5:
					if (serv.getLocations()[index_location].getErrorPages().size() != 0)
						return (serv.getLocations()[index_location].getErrorPages());
					else if (serv.getErrorPages().size() != 0)
						return (serv.getErrorPages());
					break ;
				case 6:
					if (serv.getLocations()[index_location].getAllowMethods().size() != 0)
						return (serv.getLocations()[index_location].getAllowMethods());
					else if (serv.getAllowMethods().size() != 0)
						return (serv.getAllowMethods());
					break ;
				case 7:
					if (serv.getLocations()[index_location].getCgiExt().size() != 0)
						return (serv.getLocations()[index_location].getCgiExt());
					else if (serv.getCgiExt().size() != 0)
						return (serv.getCgiExt());
					break ;
				case 8:
					if (serv.getLocations()[index_location].getCgiPathes().size() != 0)
						return (serv.getLocations()[index_location].getCgiPathes());
					else if (serv.getCgiPathes().size() != 0)
						return (serv.getCgiPathes());
					break ;
			}
		}
	}
	return (NULL);
}