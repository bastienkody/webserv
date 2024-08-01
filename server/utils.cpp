/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:48:42 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/24 17:35:02 by mmuesser         ###   ########.fr       */
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

// test absolute then non absolute + correct not perfect match
int find_location2(const std::string path, Server serv)
{
	size_t size = 0;
	int index = -1;

	// absolute match
	for(size_t i = 0; i < serv.getLocations().size(); i++)
		if (serv.getLocations()[i].getIsPathAbsolute() == true && path == serv.getLocations()[i].getPath())
				return i;
	// non absolute : perfect or longest match
	for(size_t i = 0; i < serv.getLocations().size(); i++)
	{
		std::string loc_path = serv.getLocations()[i].getPath();
		std::cout << "locpath gen:" << loc_path << std::endl;
		if (serv.getLocations()[i].getIsPathAbsolute() == true)
			continue;
		std::cout << "after is true" << loc_path << std::endl;
		if (loc_path.size() > path.size())
			continue;
		std::cout << "after sizecheck" << loc_path << std::endl;
		if (path == loc_path)
			return i;
		std::cout << "after perfect equality" << loc_path << std::endl;
		for (size_t x = 0; x < loc_path.size(); ++x)
		{
			std::cout << "path:" << path[x] << " vs locpath:" << loc_path[x] << " for x=" << x << std::endl;
			if (path[x] != loc_path[x])
				break;
			if (x > size)
			{
				size = x;
				index = i;
			}
		}
	}
	return (index);
}

/*A modifier ?*/
int find_location(const std::string path, Server serv)
{
	size_t size = 0;
	int index = -1;
	for(size_t i = 0; i < serv.getLocations().size(); i++)
	{
		if (path == serv.getLocations()[i].getPath())
		{
			if (serv.getLocations()[i].getIsPathAbsolute() == true)
				return (i);
			else
			{
				for (size_t y = i; y < serv.getLocations().size(); y++)
				{
					if (path == serv.getLocations()[i].getPath()
						&& serv.getLocations()[i].getIsPathAbsolute() == true)
						return (y);
				}	
				return (i);
			}
		}
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

std::string	find_str_data(Server serv, int index_location, std::string to_find)
{
	std::string lst_data[5] = {"root", "index", "auto_index", "body_size", "redirection"};
	for (int i = 0; i < 5; i++)
	{
		if (to_find == lst_data[i])
		{
			switch (i)
			{
				case 0:
					if (index_location > -1 && serv.getLocations()[index_location].getRoot().size() != 0)
						return (serv.getLocations()[index_location].getRoot());
					else if (serv.getRoot().size() != 0)
						return (serv.getRoot());
					break ;
				case 1:
					if (index_location > -1 && serv.getLocations()[index_location].getIndex().size() != 0)
						return (serv.getLocations()[index_location].getIndex());
					else if (serv.getIndex().size() != 0)
						return (serv.getIndex());
					break ;
				case 2:
					if (index_location > -1 && serv.getLocations()[index_location].getAutoIndex().size() != 0)
						return (serv.getLocations()[index_location].getAutoIndex());
					else if (serv.getAutoIndex().size() != 0)
						return (serv.getAutoIndex());
					break ;
				case 3:
					if (index_location > -1 && serv.getLocations()[index_location].getMaxBodySize().size() != 0)
						return (serv.getLocations()[index_location].getMaxBodySize());
					else if (serv.getMaxBodySize().size() != 0)
						return (serv.getMaxBodySize());
					break ;
				case 4:
					if (index_location > -1 && serv.getLocations()[index_location].getRedirection().size() != 0)
						return (serv.getLocations()[index_location].getRedirection());
					else if (serv.getRedirection().size() != 0)
						return (serv.getRedirection());
					break ;
			}
		}
	}
	return (std::string());
}

std::map<std::string, std::string> find_error_pages(Server serv, int index_location)
{
	if (index_location > -1 && serv.getLocations()[index_location].getErrorPages().size() != 0)
		return (serv.getLocations()[index_location].getErrorPages());
	else if (serv.getErrorPages().size() != 0)
		return (serv.getErrorPages());
	return (std::map<std::string, std::string>());
}

std::vector<std::string> find_vector_data(Server serv, int index_location, std::string to_find)
{
	std::string lst_data[3] = {"allow_methods", "cgi_ext", "cgi_pathes"};
	for (int i = 0; i < 3; i++)
	{
		if (to_find == lst_data[i])
		{
			switch (i)
			{
				case 0:
					if (index_location > -1 && serv.getLocations()[index_location].getAllowMethods().size() != 0)
						return (serv.getLocations()[index_location].getAllowMethods());
					else if (serv.getAllowMethods().size() != 0)
						return (serv.getAllowMethods());
					break ;
				case 1:
					if (index_location > -1 && serv.getLocations()[index_location].getCgiExt().size() != 0)
						return (serv.getLocations()[index_location].getCgiExt());
					else if (serv.getCgiExt().size() != 0)
						return (serv.getCgiExt());
					break ;
				case 2:
					if (index_location > -1 && serv.getLocations()[index_location].getCgiPathes().size() != 0)
						return (serv.getLocations()[index_location].getCgiPathes());
					else if (serv.getCgiPathes().size() != 0)
						return (serv.getCgiPathes());
					break ;
					
			}
		}
	}
	return (std::vector<std::string>());
}
