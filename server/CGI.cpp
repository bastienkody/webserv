/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:31:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/13 16:33:32 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include "../bastien/RequestParsing/Request.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

/*creer fonction avant cgi() qui va selon ressource request appeler cgi gerer .html*/

/*
check si j'ai tout body via content_length (en checkant _header dans Request obj)
creer env a partir des headers
build av
creer pipe, fork et dup fd pour que resultat script soit recuperable vu qu'il va ecrire sur stdout
check verb (voir url selon les scripts qu'on aura) pour exec script correspondant
launch execve
*/

/* MISE EN PLACE PROG CGI :
+ Rendre le code plus propre et pouvoir lancer des scripts
~ creer pathname pour execve
- mettre gestion erreur

- continuer recherche sur scripts cgi

*/


char **create_env(Request rq)
{
	char **env_array = new char *[rq.getHeader().size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = rq.getHeader().begin(); it != rq.getHeader().end(); it++)
	{
		std::string tmp;
		tmp.append(it->first + "=" + it->second);
		env_array[i] = strdup(tmp.c_str());
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

std::string create_path_name(Request rq)
{
	std::string pathname;

	pathname = "cgi-bin" + rq.getRql().getUrl().getPath();
	return (pathname);
}

void	exec_son(Request rq, char **av, int *pipe_fd)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	
	char **env;
	std::string pathname;
	env = create_env(rq);
	pathname = create_path_name(rq);
	std::cerr<< "pathname : "<< pathname<<std::endl;
	execve(pathname.c_str(), av, env);
	perror("Execve");
}

char	*exec_father(Request rq, int *pipe_fd)
{
	// wait(NULL);
	int status;
	char *buff;
	buff = (char *) malloc(sizeof(char) * (100 + 1));
	if (!buff)
		return (NULL);
	for (int i = 0; i < 100; i++)
		buff[i] = '\0';
	dup2(pipe_fd[0], STDIN_FILENO);
	status = read(pipe_fd[0], buff, 100);
	if (status == -1)
		return (NULL);
	std::cout<< "buff : "<< buff;
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (buff);
}

int cgi(Request rq, char **av, char **env)
{
	/*creation av
		av[0] -> nom script
		av[1] -> nom fichier demande*/

	int pipe_fd[2];
	int status;
	char *buff;

	status = pipe(pipe_fd);
	if (status == -1)
		return (perror("pipe"), -1);
	status = fork();
	if (status == -1)
		return (perror("fork"), -1);
	else if (status == 0)
		exec_son(rq, av, pipe_fd);
	else
		buff = exec_father(rq, pipe_fd);
	return (0);
}

// int	main(int ac, char **av, char **env)
// {
// 	std::string data[] = {"POST http://localhost:80/home.html?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\ncontent-length: 73\nformat: text\n\nthis is the body firstline\nthis is the body secondline (with a final lf)\n",
// 						"GET http://localhost:80/home.html?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\ncontent-length: 73\nformat: text\n\nthis is the body firstline\nthis is the body secondline (with a final lf)\n",
// 						"DELETE http://localhost:80/home.html?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\ncontent-length: 73\nformat: text\n\nthis is the body firstline\nthis is the body secondline (with a final lf)\n"};
// 	for (int i = 0; i < 3; i++)
// 	{
// 		Request rq(data[i]);
// 		cgi(rq, av, env);
// 	}
// 	return (0);
// }
