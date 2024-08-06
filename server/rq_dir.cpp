/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_dir.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:03:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/08/06 14:39:04 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"

// bool	check_auto_index(Server serv, int index_location)
// {
// 	if (serv.getLocations()[index_location].getAutoIndex() == "on")
// 		return (true);
// 	else if (serv.getLocations()[index_location].getAutoIndex() == "off")
// 		return (false);
// 	if (serv.getAutoIndex() == "on")
// 		return (true);
// 	else
// 		return (false);
// }

std::string read_index(Request rq, std::string index)
{
	int status = check_file(rq, "www", 1);
	if (status > 0)
		return ("Error");
	std::ifstream file(index.c_str());
	if (!file)
		return ("Error");
	std::string buff;
	while (!file.eof())
	{
		std::string tmp;
		file >> tmp;
		buff += tmp + "\n";
	}
	return (buff);
}

std::string	create_index(Server serv, int index_location)
{
	std::string root = find_str_data(serv, index_location, "root");
	if (root.size() == 0)
		return ("Error"); /*500 ??*/
	DIR *my_dir = opendir(root.c_str());
	if (!my_dir)
		return ("Error"); /*500*/
	std::string buff;
	struct dirent *dir_struct;
	while (true)
	{
		dir_struct = readdir(my_dir);
		if (dir_struct == NULL)
			break ;
		buff += dir_struct->d_name;
		buff += "\n";
	}
	if (closedir(my_dir) == -1)
		return ("Error"); /*500*/
	return (buff);
}

void	rq_dir(Response *rp, Request rq, ConfigFile config, Server serv, int index_location)
{
	std::string auto_index = find_str_data(serv, index_location, "auto_index");
	if (auto_index.size() == 0)
		return (rp->setBody("Error")); /*500 ??*/
	if (serv.getLocations()[index_location].getIndex().size() != 0)
		return (rp->setBody(read_index(rq, serv.getLocations()[index_location].getIndex())));
	else if (serv.getIndex().size() != 0)
		return (rp->setBody(read_index(rq, serv.getIndex())));
	else if (auto_index == "on")
		return (rp->setBody(create_index(serv, index_location)));
	else
		exec_rq_error(rq, config, 403);
}