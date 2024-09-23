/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/23 17:26:26 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../include/CGI.hpp"
#include <dirent.h>
#include <sstream>

/*ajouter Location obj pour check methods allows*/

int	check_cgi_ext(Server serv, std::string path, int index_loc)
{
	std::string ext;
	
	if (path.find('.') == std::string::npos)
	{
		std::cerr<< "BLABLA 1"<<std::endl;
		return 0;
	}
	ext = &path[path.rfind('.')];
	std::vector<std::string> cgi_ext = find_vector_data(serv, index_loc, "cgi_ext");
	if (cgi_ext.size() == 0)
	{
		std::cerr<< "BLABLA 2"<<std::endl;
		return (0);
	}
	for (std::vector<std::string>::iterator it = cgi_ext.begin(); it != cgi_ext.end(); it++)
	{
		if (*it == ext)
		{
			std::cerr<< "BLABLA 3"<<std::endl;
			return (1);
		}
	}
	std::cerr<< "BLABLA 4"<<std::endl;
	return (0);
}

/*CHECK CGI EXT RENVOIE PLUS BONNE VALEUR ?????*/

Response	exec_rq(Request rq, ConfigFile config, int index_serv, int index_loc)
{
	Response rp;
	DIR	*dir_test = NULL;

	if (index_loc == -1)
		return exec_rq_error(rq, config, 404, index_serv, index_loc);

	std::string path = concatenate_root_path(rq, config, index_serv, index_loc);
	if (path.find("/..") != std::string::npos)
		return std::cerr<<"Security protection: deny any \"../\" in URL"<<std::endl, exec_rq_error(rq, config, 400, index_serv, index_loc);

	int	redirect_code = is_url_redirected(rq.getRql().getUrl().getPath(), path, config.getServers()[index_serv], index_loc);
	if (redirect_code != 0)
	{
		rp.setLineState(redirect_code);
		rp.setHeader(rq, config, index_serv, index_loc);
		rp.setLocation(path);
		if (redirect_code == 301)
			rp.setBody("<h1>Error 301: Moved Permanently (redirection) /h1>", "html");
		else
			rp.setBody("<h1>Error 302: Found (temporary redirection) /h1>", "html");
		return rp;

	}
	if (path.size() == 0)
		return exec_rq_error(rq, config, 500, index_serv, index_loc);

	if (check_cgi_ext(config.getServers()[index_serv], rq.getRql().getUrl().getPath(), index_loc) == 1)
	{
		//if (DEBUGP) {std::cerr<< "Enter CGI :"<<std::endl;}
		CGI cgi(&rp, rq, config, index_serv, index_loc);
	}
	else if (rq.getRql().getVerb() == "GET" && (path[path.size() - 1] == '/' || (dir_test = opendir(path.c_str())) != NULL))
	{
		if (dir_test)
			closedir(dir_test);
		//if (DEBUGP) {std::cerr<< "Enter rq_dir :"<<std::endl;}
		rq_dir(&rp, rq, config, config.getServers()[index_serv], index_loc, index_serv);
	}
	else
	{
		//if (DEBUGP) {std::cerr<< "Enter rq_html :"<<std::endl;}
		rq_html(&rp, rq, path, config, index_serv, index_loc);
	}
	return (rp);
}

// used by exeq_rq_error if no error pages set in configfile
std::string	fetch_default_error_page(int code)
{
	switch (code)
	{
		case 400:
			return DEFAULT_400;
		case 403:
			return DEFAULT_403;
		case 404:
			return DEFAULT_404;
		case 405:
			return DEFAULT_405;
		case 413:
			return DEFAULT_413;
		case 500:
			return DEFAULT_500;
		case 501:
			return DEFAULT_501;
		case 505:
			return DEFAULT_505;
		default:
			return "";
	}
}

Response	exec_rq_error(Request rq, ConfigFile config, int code, int index_serv, int index_loc)
{
	Response		rp;
	std::stringstream	sscode;
	sscode << code;
	rp.setLineState(code);
	rp.setHeader(rq, config, index_serv, index_loc);

	std::string				path;
	std::map<std::string, std::string>	err_pages = find_error_pages(config.getServers()[index_serv], index_loc);
	if (err_pages.size() != 0 && err_pages[sscode.str()].size() != 0)
		path = err_pages[sscode.str()];
	else
		path = fetch_default_error_page(code);
	if (path.size() > 0 && access(path.c_str(), R_OK) == 0)
	{
		std::ifstream		infile(path.c_str());
		std::stringstream	sstr;
		sstr << infile.rdbuf();
		rp.setBody(sstr.str(), "html");
	}
	else // could not retrieve any error pages
	{
		std::string	tmp(sscode.str() + ' ');
		tmp += StatusCode::getPhrase(code);
		rp.setBody(tmp, "html");
	}
	return (rp);
}

