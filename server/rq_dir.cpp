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

std::string read_index(std::string rooted_path, std::string index_filename, int *code)
{
	std::string path(rooted_path + index_filename);
	int status = check_file(path, 1);
	if (status > 0)
		return (*code = 404, "Error");
	std::ifstream file(path.c_str());
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

std::string	create_index(std::string rooted_path, std::string user_path, int *code)
{
	DIR *my_dir = opendir(rooted_path.c_str());
	if (!my_dir)
		return (*code = 403, "Error"); // 500 plutot nan?
	
	std::vector<std::string> data;
	data.push_back("<!DOCTYPE html><html lang=\"en\">\n<head><title>Index of "+ user_path + "</title></head><body>" + "<h2>Index of " + user_path + "</h2><hr>");

	struct dirent *dir_struct;
	while ((dir_struct = readdir(my_dir)) != NULL)
	{
		std::string line("<a href=\"");
		line += user_path + (user_path[user_path.size()-1]=='/'?"":"/");
		line += std::string(dir_struct->d_name) + "\">" + std::string(dir_struct->d_name) + "</a><br>";
		data.push_back(line);
	}
	if (closedir(my_dir) == -1)
		return (*code = 500, "Error");

	std::sort(data.begin(), data.end());
	data.push_back("<hr></body></html>");
	std::string buff;
	for (std::vector<std::string>::iterator it = data.begin(); it != data.end(); ++it)
		buff.append(*it);
	return (buff);
}

/*
 SERVIR LES SOUS ODSSIERS (via path, pas juste loc + root)
 SERVIR INDEX.HTML si pas de de directive index
 directive index : ne sert que pour url == location, pas les sous dossiers
*/

void	rq_dir(Response *rp, Request rq, ConfigFile config, Server serv, int id_loc, int id_serv)
{
	int		code = 0;
	std::string	buff, loc_index = serv.getLocations()[id_loc].getIndex(), serv_index = serv.getIndex();
	bool		path_is_loc = (rq.getRql().getUrl().getPath() == serv.getLocations()[id_loc].getPath() ? true : false);
	bool		auto_index = find_str_data(serv, id_loc, "autoindex") == "on" ? true : false;

	std::cout << "Path:" +rq.getRql().getUrl().getPath() << std::endl;
	std::cout << "loc_path:" + serv.getLocations()[id_loc].getPath() << std::endl;
	std::cout << "Path==loc?" << path_is_loc << std::endl;
	std::cout << "Autoindex?" << path_is_loc << std::endl;

	if (path_is_loc && ( loc_index.size() != 0 || serv_index.size() != 0 ) )
	{
		if (loc_index.size() != 0)
			buff = read_index(concatenate_root_path(rq, config, id_serv, id_loc), loc_index, &code);
		else
			buff = read_index(concatenate_root_path(rq, config, id_serv, id_loc), serv_index, &code);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, code, id_serv, id_loc);
			return ;
		}
	}
	else if (auto_index) // look for index.html, if not create index
	{
		std::string	path = concatenate_root_path(rq, config, id_serv, id_loc);
		buff = read_index(path, "/index.html", &code); // pb de slash ?
		if (code == 404)
			buff = create_index(path, rq.getRql().getUrl().getPath(), &code);
		if (buff == "Error")
		{
			*rp = exec_rq_error(rq, config, code, id_serv, id_loc);
			return ;
		}
	}
	else
	{
		*rp = exec_rq_error(rq, config, 403, id_serv, id_loc);
		return ;
	}
	rp->setLineState(200);
	rp->setHeader(rq, config, id_serv, id_loc);
	rp->setBody(buff, "html");
	return ;
}
