/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/08/07 14:16:58 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../include/CGI.hpp"
#include "../include/Exception.hpp"
#include <sstream>

/*ajouter Location obj pour check methods allows*/

int	check_cgi_ext(Server serv, std::string path, int index_loc)
{
	std::string ext;
	
	ext = path.rfind('.');
	std::vector<std::string> cgi_ext = find_vector_data(serv, index_loc, "cgi_ext");
	if (cgi_ext.size() == 0)
		return (0);
	for (std::vector<std::string>::iterator it = cgi_ext.begin(); it != cgi_ext.end(); it++)
	{
		if (*it == ext)
			return (1);
	}
	return (0);
}

Response	exec_rq(Request rq, ConfigFile config, int index_serv, int index_loc)
{
	if (index_loc == -1)
		return exec_rq_error(rq, config, 404, index_serv, index_loc);
	Response rp;
	std::string path = rq.getRql().getUrl().getPath();
	try{
		if (check_cgi_ext(config.getServers()[index_serv], path, index_loc) == 1)
			CGI cgi(&rp, rq, config, index_serv, index_loc);
		else if (path[path.size() - 1] == '/')
			rq_dir(&rp, rq, config, config.getServers()[index_serv], index_loc, index_serv);
		else
			rq_html(&rp, rq, config, index_serv, index_loc);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
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

	rp.setLineState(code);					// create status line
	rp.setHeader(rq, config, index_serv, index_loc);	// create headers
	// create body (via error pages in config file or default pages)
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
	return (rp);
}
// int main(void)
// {
// 	std::string request = "GET http://localhost:80/home.txt?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\nformat: text\n";

// 	Request rq(request);
// 	rq.print();
// 	exec_rq(rq);
// }
