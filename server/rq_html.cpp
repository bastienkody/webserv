/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_html.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:20:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/23 15:46:25 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../RequestParsing/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include <dirent.h>
#include <fstream>

/*gerer directory (comment ??)*/

// redirect: on sert le fichier redirige ce qui est inutil
// on pourrait passer par un booleen pour savoir si on doit servir ou non
void	get_html(Response *rp, Request rq, std::string path, ConfigFile config, int index_serv, int index_loc, bool is_redirect)
{
	if (is_redirect)
	{
		rp->setHeader(rq, config, index_serv, index_loc);
		return ;
	}

	if (DEBUGP) {std::cout << "from rq_html path: " + path << std::endl;}
	int status;
	std::string tmp, buff;
	status = check_file(path, 1);
	if (status > 0)
	{
		*rp = exec_rq_error(rq, config, 404, index_serv, index_loc);
		return ;
	}
	std::ifstream my_html(path.c_str());
	if (!my_html)
	{
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	while (std::getline(my_html, tmp))
	{
		if(!my_html.eof())
			buff.append(tmp + '\n');
		else
			buff.append(tmp);
	}
	rp->setLineState(200);
	rp->setHeader(rq, config, index_serv, index_loc);
	rp->setBody(buff, path.substr(path.rfind('.') + 1, path.size() - 1)); // to get the real extension
}

void	post_html(Response *rp, Request rq, std::string path, ConfigFile config, int index_serv, int index_loc)
{
	if (access(path.c_str(), F_OK) == 0)
	{
		if (DEBUGP) {std::cout << "File to be posted already exists (400)" << std::endl;}
		*rp = exec_rq_error(rq, config, 400, index_serv, index_loc);
		return ;
	}
	DIR *dir_test = NULL;
	std::string	last_dir(path.substr(0, path.rfind('/') + 1)); // if url ends with a dir but no '/', catch just above !
	if ( (dir_test = opendir(last_dir.c_str())) == NULL )
	{
		//std::cout<<"lastdir:"+last_dir<<std::endl;
		if (DEBUGP) {std::cout << "Given path of post request does not exist (404)" << std::endl;}
		*rp = exec_rq_error(rq, config, 404, index_serv, index_loc);
		return ;
	}
	closedir(dir_test);

	std::ofstream my_html(path.c_str());
	if (!my_html)
	{
		if (DEBUGP) {std::cout << "File creation failed (500)" << std::endl;}
		*rp = exec_rq_error(rq, config, 500, index_serv, index_loc);
		return ;
	}
	my_html << rq.getBody();
	rp->setLineState(201);
	rp->setHeader(rq, config, index_serv, index_loc);
	rp->setLocation(rq.getRql().getUrl().getPath());
}

// idem redirect post
void	delete_html(Response *rp, Request rq, std::string path, ConfigFile config, int index_serv, int index_loc)
{
	int status;
	status = check_file(path, 1);
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
	rp->setLineState(204);
	rp->setHeader(rq, config, index_serv, index_loc);
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

void	rq_html(Response *rp, Request rq, std::string path, ConfigFile config, int index_serv, int index_loc, bool is_redirect)
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
				case 0: get_html(rp, rq, path, config, index_serv, index_loc, is_redirect); break;
				case 1: post_html(rp, rq, path, config, index_serv, index_loc); break;
				case 2: delete_html(rp, rq, path, config, index_serv, index_loc); break;
			}
			return ;
		}
	}
	*rp = exec_rq_error(rq, config, 405, index_serv, index_loc);
}
