/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rq.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/14 17:22:34 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../include/CGI.hpp"
#include "../include/Exception.hpp"

/*ajouter Location obj pour check methods allows*/

int	check_body_size(Request rq)
{
	int body_length = rq.getBody().size();

	for (std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin(); it != rq.getHeader().end(); ++it)
	{
		if (it->first == "content-length")
		{
			if (atoi(it->second.c_str()) != body_length)
				return (std::cout<< "Error: wrong body size"<<std::endl, -1);
		}
	}
	return (0);
}

Response	exec_rq(Request rq, ConfigFile config)
{
	Response rp;
	std::string tmp;
	std::string path = rq.getRql().getUrl().getPath();

	(void) config;
	tmp = &path[path.size() - 3];
	try{
		if (tmp == ".py")
				CGI cgi(&rp, rq);
		else if (path[path.size() - 1] == '/')
			int blabla;// rq_dir(&rp, rq);
		else
			rq_html(&rp, rq);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
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