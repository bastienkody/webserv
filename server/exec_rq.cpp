/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/26 14:48:12 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../include/CGI.hpp"
#include "../include/Exception.hpp"

/*ajouter Location obj pour check methods allows*/

int	check_cgi_ext(Server serv, std::string path, int index_location)
{
	std::string ext;

	ext = path.rfind('.');
	for (size_t i = 0; i < serv.getLocations()[index_location].getCgiExt().size(); i++)
	{
		if (ext == serv.getLocations()[i].getCgiExt()[i])
			return (1);
	}
	for (size_t i = 0; i < serv.getCgiExt().size(); i++)
	{
		if (ext == serv.getCgiExt()[i])
			return (1);
	}
	return (0);
}

Response	exec_rq(Request rq, ConfigFile config, int index_serv)
{
	Response rp;
	int index_location = find_location(rq.getRql().getUrl().getPath(), config.getServers()[index_serv]);
	//if (index_location == -1)
	//	return 404
	std::string path = config.getServers()[index_serv].getLocations()[index_location].getPath();

	try{
		if (check_cgi_ext(config.getServers()[index_serv], path, index_location) == 1)
				CGI cgi(&rp, rq);
		else if (path[path.size() - 1] == '/')
			rq_dir(&rp, rq, config, config.getServers()[index_serv], index_location);
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
