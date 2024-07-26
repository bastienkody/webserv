/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_html.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:20:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/26 19:14:59 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../RequestParsing/Request.hpp"
#include "../include/Exception.hpp"
#include <fstream>

/*gerer directory (comment ??)*/

void	get_html(Response *rp, Request rq)
{
	std::string buff;
	std::string path = "www" + rq.getRql().getUrl().getPath();
	int status;

	/*check dir -> return 0 si pas un dir*/
	status = check_file(rq, "www", 1);
	if (status > 0)
		return ;
	std::ifstream my_html(path.c_str());
	if (!my_html)
		return (rp->setBody("Error")); /*500*/
	while (!my_html.eof())
	{
		std::string tmp;
		my_html >> tmp;
		buff += tmp + "\n";
	}
	rp->setBody(buff);
}

void	post_html(Response *rp, Request rq) /*je sais pas encore comment faire*/
{
	std::string path = "www" + rq.getRql().getUrl().getPath();

	/*a utiliser si permet pas de post un fichier avec le meme nom qu'un deja existant*/
	// status = check_file(rq, "www", 0);
	// if (status == 0)
	// 	return ;
	std::ofstream my_html(path.c_str());
	if (!my_html)
		return (rp->setBody("Error")); /*500*/
	my_html << rq.getBody();
}

void	delete_html(Response *rp, Request rq)
{
	std::string path = rq.getRql().getUrl().getPath();
	int status;

	/*check dir -> return 0 si pas un dir*/
	status = check_file(rq, "www", 1);
	if (status > 0)
		return (rp->setBody("Error")); /*500*/
	path = "www" + path;
	status = remove(path.c_str());
	if (status != 0)
		return (rp->setBody("Error")); /*500*/
}

void	rq_html(Response *rp, Request rq)
{
	std::string method[3] = {"GET", "POST", "DELETE"};

	for (int i = 0; i < 3; i++)
	{
		if (rq.getRql().getVerb() == method[i])
		{
			switch(i)
			{
				case 0: get_html(rp, rq); break;
				case 1: post_html(rp, rq); break;
				case 2: delete_html(rp, rq); break;
			}
			return ;
		}
	}
	throw Exception(4);
}
