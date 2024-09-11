/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_dir.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:03:19 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/11 14:11:21 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"

std::string read_index(Server serv, std::string index, int index_loc, int *code
)
{
	std::string root = find_str_data(serv, index_loc, "root");
	if (root.size() == 0)
		return (*code = 500, "Error");
	index = root + index;
	int status = check_file(index, 1);
	if (status > 0)
	{
		std::cerr<< "status : "<< status<<std::endl;
		return (*code = 404, "Error");
	}
	std::ifstream file(index.c_str());
	if (!file)
		return (*code = 500, "Error");
	std::string tmp, buff;
	while(std::getline(file, tmp))
	{
		if (!file.eof())
			buff.append(tmp + '\n');
		else
			buff.append(tmp);
	}
	return (buff);
}

std::string	create_index(Server serv, int index_loc, int *code)
{
	std::string root = find_str_data(serv, index_loc, "root");
	if (root.size() == 0)
		return (*code = 500, "Error");
	DIR *my_dir = opendir(root.c_str());
	if (!my_dir)
		return (*code = 403, "Error");
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
		return (*code = 500, "Error");
	return (buff);
}

void	rq_dir(Response *rp, Request rq, ConfigFile config, Server serv, int index_loc, int index_serv)
{
	std::string auto_index = find_str_data(serv, index_loc, "auto_index");
	std::string buff;
	std::string loc_index = serv.getLocations()[index_loc].getIndex();
	int code;

	if (loc_index.size() != 0)
	{
		buff = read_index(serv, loc_index, index_loc, &code);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, code, index_serv, index_loc);
			return ;
		}
	}
	else if (serv.getIndex().size() != 0)
	{
		buff = read_index(serv, serv.getIndex(), index_loc, &code);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, code, index_serv, index_loc);
			return ;
		}
	}
	else if (auto_index == "on")
	{
		buff = create_index(serv, index_loc, &code);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, code, index_serv, index_loc);
			return ;
		}
	}
	else
	{
		*rp = exec_rq_error(rq, config, 403, index_serv, index_loc);
		return ;
	}
	rp->setLineState(200);
	rp->setHeader(rq, config, index_serv, index_loc);
	rp->setBody(buff, "html");
	return ;
}
