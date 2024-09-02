/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_dir.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:03:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/02 15:57:14 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"

std::string read_index(Server serv, Request rq, std::string index, int index_loc)
{
	(void) rq;
	std::string root = find_str_data(serv, index_loc, "root");
	if (root.size() == 0)
		return ("Error");
	std::cerr<< "index : " << index<<std::endl;
	index = root + index;
	int status = check_file(index, 1);
	if (status > 0)
	{
		std::cerr<< "status : "<< status<<std::endl;
		return ("Error");
	}
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
		buff += "<br>";
	}
	if (closedir(my_dir) == -1)
		return ("Error");
	return (buff);
}

void	rq_dir(Response *rp, Request rq, ConfigFile config, Server serv, int index_loc, int index_serv)
{
	std::string auto_index = find_str_data(serv, index_loc, "auto_index");
	std::string buff;
	std::string loc_index = serv.getLocations()[index_loc].getIndex();
	if (auto_index.size() == 0)
	{
		std::cerr<< "Blabla 3"<<std::endl;
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	if (loc_index.size() != 0)
	{
		buff = read_index(serv, rq, loc_index, index_loc);
		if (buff == "Error")
		{
			std::cerr<< "Blabla 4"<<std::endl;
			*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
			return ;
		}
	}
	else if (serv.getIndex().size() != 0)
	{
		buff = read_index(serv, rq, serv.getIndex(), index_loc);
		if (buff == "Error")
		{
			std::cerr<< "Blabla 5"<<std::endl;
			*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
			return ;
		}
	}
	else if (auto_index == "on")
	{
		buff = create_index(serv, index_loc);
		if (buff == "Error")
		{
			std::cerr<< "Blabla 6"<<std::endl;
			*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
			return ;
		}
	}
	else
	{
		std::cerr<< "Blabla 7"<<std::endl;
		*rp = exec_rq_error(rq, config, 403, index_serv, index_loc);
	}
	rp->setLineState(200);
	rp->setHeader(rq, config, index_serv, index_loc);
	rp->setBody(buff, "html");
	return ;
}