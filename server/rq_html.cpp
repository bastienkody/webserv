/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_html.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:20:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/08/07 14:32:48 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../RequestParsing/Request.hpp"
#include "../include/Exception.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include <fstream>

/*gerer directory (comment ??)*/

void	get_html(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc)
{
	std::string root = find_str_data(config.getServers()[index_serv], index_loc, "root");
	std::cerr<< "root : " << root << std::endl;
	if (root.size() == 0)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	std::string buff;
	std::string path = root + rq.getRql().getUrl().getPath();
	int status;

	/*check dir -> return 0 si pas un dir*/
	status = check_file(rq, root, 1);
	if (status > 0)
	{
		std::cerr<< "status : " << status<<std::endl;
		*rp = exec_rq_error(rq, config, 404, index_serv, index_loc);
		return ;
	}
	std::ifstream my_html(path.c_str());
	if (!my_html)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	while (!my_html.eof())
	{
		std::string tmp;
		my_html >> tmp;
		buff += tmp + "\n";
	}
	rp->setLineState(200);
	rp->setHeader(rq, config, index_serv, index_loc);
	rp->setBody(buff, "html");
}

void	post_html(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc)
{
	std::string root = find_str_data(config.getServers()[index_serv], index_loc, "root");
	if (root.size() == 0)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	std::string path = root + rq.getRql().getUrl().getPath();

	/*a utiliser si permet pas de post un fichier avec le meme nom qu'un deja existant*/
	// status = check_file(rq, root, 0);
	// if (status == 0)
	// 	return ;
	std::ofstream my_html(path.c_str());
	if (!my_html)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	my_html << rq.getBody();
	rp->setLineState(201);
	rp->setHeader(rq, config, index_serv, index_loc);
}

void	delete_html(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc)
{
	int status;
	std::string root = find_str_data(config.getServers()[index_serv], index_loc, "root");
	if (root.size() == 0)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	std::string path = root + rq.getRql().getUrl().getPath();
	/*check dir -> return 0 si pas un dir*/
	status = check_file(rq, root, 1);
	if (status > 0)
	{
		*rp = exec_rq_error(rq, config, 404, index_serv, index_loc);
		return ;
	}
	status = remove(path.c_str());
	if (status != 0)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
}

int	check_allow(Server serv, int index_loc, std::string method)
{
	std::vector<std::string> allow = find_vector_data(serv, index_loc, "allow_methods");
	if (allow.size() == 0)
		return (1);
	for (std::vector<std::string>::iterator it = allow.begin(); it != allow.end(); it++)
	{
		if (method == *it)
			return (1);
	}
	return (0);
}

void	rq_html(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc)
{
	std::string method[3] = {"GET", "POST", "DELETE"};

	for (int i = 0; i < 3; i++)
	{
		if (rq.getRql().getVerb() == method[i])
		{
			if (check_allow(config.getServers()[index_serv], index_loc, method[i]) == 0)
				break ;
			switch(i)
			{
				case 0: get_html(rp, rq, config, index_serv, index_loc); break;
				case 1: post_html(rp, rq, config, index_serv, index_loc); break;
				case 2: delete_html(rp, rq, config, index_serv, index_loc); break;
			}
			return ;
		}
	}
	*rp = exec_rq_error(rq, config, 405, index_serv, index_loc);
}
