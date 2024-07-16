/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/16 17:24:52 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../include/CGI.hpp"
#include "../include/Exception.hpp"

/*ajouter Location obj pour check methods allows*/

int	check_cgi_ext(std::string path, Server serv)
{
	std::string ext;

	for (size_t i = 0; i < path.size(); i++)
	{
		if (path[i] == '.')
			ext = &path[i];
	}
	for (size_t i = 0; i < serv.getLocations().size(); i++)
	{
		if (path == serv.getLocations()[i].getPath())
		{
			for (size_t y = 0; y < serv.getLocations()[i].getCgiExt().size(); y++)
			{
				if (ext == serv.getLocations()[i].getCgiExt()[y])
					return (1);
			}
			return (0);
		}
		else
		{
			for (size_t y = 0; y < serv.getCgiExt().size(); y++)
			{
				if (ext == serv.getCgiExt()[y])
					return (1);
			}
			return (0);
		}
	}
	return (0);
}

Response	exec_rq(Request rq, ConfigFile config, int index_serv)
{
	Response rp;
	std::string path = config.getServers()[index_serv].getLocations()[find_location(rq.getRql().getUrl().getPath(), config.getServers()[index_serv])].getPath();

	try{
		if (check_cgi_ext(path, config.getServers()[index_serv]) == 1)
				CGI cgi(&rp, rq);
		else if (path[path.size() - 1] == '/')
			rq_dir(&rp, rq);
		else
			rq_html(&rp, rq);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return (rp);
}

Response	exec_rq_error(__attribute__((unused))Request rq, __attribute__((unused))ConfigFile config, int code)
{
	Response		rp;

	// create status line
	rp.setLineState(code);
	// create header
	rp.setHeader(rq, config);
	
	// check if error page exists in config file to fulfill body; if not no body?
	
	return (rp);
}
// int main(void)
// {
// 	std::string request = "GET http://localhost:80/home.txt?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\nformat: text\n";

// 	Request rq(request);
// 	rq.print();
// 	exec_rq(rq);
// }
