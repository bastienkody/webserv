/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:31:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/06 16:59:46 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include "../bastien/RequestParsing/Request.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

/*
recuperer Request object
check si j'ai tout body via content_length (en checkant _header dans Request obj)
creer pipe, fork et dup fd pour que resultat script soit recuperable vu qu'il va ecrire sur stdout
check verb (voir url selon les scripts qu'on aura) pour exec script correspondant
launch execve
*/

/* MISE EN PLACE PROG CGI :
- lire le contenu de pipe_fd[0] sans read
- Rendre le code plus propre et pouvoir lancer au moins GET POST ET DELETE
- Faire scripts pour methodes citees au dessus
- mettre gestion erreur
- continuer recherche sur scripts cgi

*/

int	cgi(Request rq, char **av, char **env)
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
	
	int pipe_fd[2];
	int status;

	status = pipe(pipe_fd);
	if (status == -1)
		return (perror("pipe"), -1);
	status = fork();
	if (status == -1)
		return (perror("fork"), -1);
	else if (status == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		if (rq.getRql().getVerb() == "POST")
			execve("cgi-bin/test.py", av, env);
		return (perror("Execve"), -1);
	}
	else
	{
		wait(NULL);
		char *buff;
		buff = (char *) malloc(sizeof(char) * (100 + 1));
		if (!buff)
			return (std::cout<< "Error: Malloc"<<std::endl, -1);
		for (int i = 0; i < 100; i++)
			buff[i] = '\0';
		dup2(pipe_fd[0], STDIN_FILENO);
		status = read(pipe_fd[0], buff, 100);
		if (status == -1)
			return (perror("read"), -1);
		std::cout<< "buff : "<< buff;
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		free(buff);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	std::string data = "POST http://localhost:80/home.html?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\ncontent-length: 73\nformat: text\n\nthis is the body firstline\nthis is the body secondline (with a final lf)\n";
	Request rq(data);
	cgi(rq, av, env);
	return (0);
}

// int main(int ac, char **av, char **env)
// {
// 	int status;
// 	status = fork();
// 	if (status == 0)
// 	{
// 		execve("cgi-bin/test.py", av, env);
// 		perror("Execve");
// 	}
// 	wait(NULL);
// 	return (0);
// }
