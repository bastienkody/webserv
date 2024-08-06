/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_dir.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:03:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/08/06 17:39:50 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"

// bool	check_auto_index(Server serv, int index_loc)
// {
// 	if (serv.getLocations()[index_loc].getAutoIndex() == "on")
// 		return (true);
// 	else if (serv.getLocations()[index_loc].getAutoIndex() == "off")
// 		return (false);
// 	if (serv.getAutoIndex() == "on")
// 		return (true);
// 	else
// 		return (false);
// }

std::string read_index(Server serv, Request rq, std::string index, int index_loc)
{
	std::string root = find_str_data(serv, index_loc, "root");
	if (root.size() == 0)
		return ("Error");
	int status = check_file(rq, root, 1);
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

std::string	create_index(Server serv, int index_loc)
{
	std::string root = find_str_data(serv, index_loc, "root");
	if (root.size() == 0)
		return ("Error");
	DIR *my_dir = opendir(root.c_str());
	if (!my_dir)
		return ("Error");
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
		return ("Error");
	return (buff);
}

void	rq_dir(Response *rp, Request rq, ConfigFile config, Server serv, int index_loc, int index_serv)
{
	std::cerr<< "BLABLA 4"<<std::endl;
	std::string auto_index = find_str_data(serv, index_loc, "auto_index");
	std::string buff;
	if (auto_index.size() == 0)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	if (serv.getLocations()[index_loc].getIndex().size() != 0)
	{
		std::cerr<< "BLABLA 5"<<std::endl;
		buff = read_index(serv, rq, serv.getLocations()[index_loc].getIndex(), index_loc);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
			return ;
		}
		return (rp->setBody(buff, "text/html"));
	}
	else if (serv.getIndex().size() != 0)
	{
		std::cerr<< "BLABLA 6"<<std::endl;
		buff = read_index(serv, rq, serv.getLocations()[index_loc].getIndex(), index_loc);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
			return ;
		}
		return (rp->setBody(buff, "text/html"));
	}
	else if (auto_index == "on")
	{
		std::cerr<< "BLABLA 7"<<std::endl;
		buff = create_index(serv, index_loc);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
			return ;
		}
		return (rp->setBody(buff, "text/html"));
	}
	else
		exec_rq_error(rq, config, 405, index_serv, index_loc);
}