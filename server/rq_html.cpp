/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rq_html.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:20:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/28 18:39:30 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../RequestParsing/Request.hpp"
#include "../include/Exception.hpp"

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
}

void	post_html(Response *rp, Request rq) /*je sais pas encore comment faire*/
{}

void	delete_html(Response *rp, Request rq)
{
	std::string path = rq.getRql().getUrl().getPath();
	int pipe_fd[2];
	int status;

	/*check dir -> return 0 si pas un dir*/
	status = check_file(rq, "www", 2);
	if (status > 0)
		return (rp->setBody("Error"));
	status = pipe(pipe_fd);
	if (status == -1)
		return (rp->setBody("Error"));
	status = fork();
	if (status == -1)
		return (rp->setBody("Error"));
	else if (status == 0)
	{
		char **env = create_env(rq);
		std::string tmp = "rm";
		char **av = create_av(rq);
		execve("/bin/rm", av, env);
		rp->setBody("Error");
		perror("Execve");
		exit(1);
	}
	wait(NULL);
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
	throw Exception(4);
}