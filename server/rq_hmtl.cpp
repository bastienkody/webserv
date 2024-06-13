/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_hmtl.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:20:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/13 16:28:12 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include "../bastien/RequestParsing/Request.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fstream>

/*gerer directory (comment ??)*/

void	get_html(Response *rp, Request rq) 
{
	/*check si get is allow avec Location obj*/
	std::string buff;
	std::string path = "www" + rq.getRql().getUrl().getPath();
	if (access(path.c_str(), R_OK) == -1)
		return (rp->setBody("Error"));

	std::ifstream my_html(path);
	if (!my_html)
		return (rp->setBody("Error"));
	while (!my_html.eof())
	{
		std::string tmp;
		my_html >> tmp;
		buff += tmp + "\n";
	}
	rp->setBody(buff);
	/*set rp header ?*/
}

void post_html(Response *rp, Request rq)
{
	/*check si ressource existe et si post is allow avec Location obj*/
}

void delete_html(Response *rp, Request rq)
{
	/*check si ressource existe et si delete is allow avec Location obj*/
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
				case 0: get_html(rp, rq);
				case 1: post_html(rp, rq);
				case 2: delete_html(rp, rq);
			}
			return ;
		}
	}
	std::cout<< "This method is not supported"<<std::endl;
}