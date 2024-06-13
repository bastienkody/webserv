/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_rq_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/13 16:03:38 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include "../include/Response.hpp"
#include "../bastien/RequestParsing/Request.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fstream>

int cgi(Request rq, char **av, char **env);

/*ajouter Location obj et envoyer a get/post/delete html pour check methods allows*/

int	check_body_size(Request rq)
{
	int body_length = rq.getBody().size();

	for (std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin(); it!=rq.getHeader().end(); ++it)
	{
		if (it->first == "content-length")
		{
			if (atoi(it->second.c_str()) != body_length)
				return (std::cout<< "Error: wrong body size"<<std::endl, -1);
		}
	}
	return (0);
}

Response	post_rq_parsing(Request rq)
{
	Response rp;
	std::string buff;
	std::string tmp;
	std::string path = rq.getRql().getUrl().getPath();

	if (check_body_size(rq) == -1)
		return (-1);
	for (size_t i = 0; i < path.size(); i++)
	{
		if (path[i] == '.')
		{
			tmp = &path[i];
			break ;
		}
	}
	if (tmp == ".py")
		cgi(&rp, rq, NULL, NULL);
	else
		rq_html(&rp, rq, path);
	/*creer response*/
	return (rp);
}

int main(void)
{
	std::string request = "POST http://localhost:80/home.txt?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\ncontent-length: 73\nformat: text\n\nthis is the body firstline\nthis is the body secondline (with a final lf)\n";

	Request rq(request);
	post_parsing(rq);
}