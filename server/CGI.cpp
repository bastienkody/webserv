/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:50:31 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/02 15:16:02 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"
#include "../include/server.hpp"

CGI::CGI(void){}
CGI::~CGI(void){}
CGI::CGI(const CGI &obj){*this = obj;}

CGI & CGI::operator=(const CGI &obj)
{
	if (this != &obj)
	{
		this->_rp = obj.getRp();
		this->_rq = obj.getRq();
	}
	return (*this);
}

Response	*CGI::getRp()	const {return _rp;}
Request		 CGI::getRq()	const {return _rq;}

CGI::CGI(Response *rp, Request rq) : _rp(rp), _rq(rq)
{
	int status;
	int pipe_fd[2];

	/*check dir -> return 0 si pas un dir ???*/
	status = check_file(this->_rq, "cgi-bin", 2);
	if (status > 0)
		throw Exception(1);
	status = pipe(pipe_fd);
	if (status == -1)
		throw Exception(2);
	status = fork();
	if (status == -1)
		throw Exception(3);
	else if (status == 0)
		this->exec_son(pipe_fd);
	else
		this->exec_father(pipe_fd);
}

void	CGI::exec_son(int *pipe_fd)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	
	std::string name = _rq.getRql().getUrl().getPath();
	std::string pathname = "cgi-bin" + name;
	char **env = create_env();
	char **av = create_av();
	execve(pathname.c_str(), av, env);
	delete [] env;
	perror("Execve");
	exit(1);
}

/*definir limite pour reponse body*/
void	CGI::exec_father(int *pipe_fd)
{
	// wait(NULL);
	int status;
	char *buff;
	buff = (char *) malloc(sizeof(char) * (100 + 1));
	if (!buff)
		return ;
	for (int i = 0; i < 100; i++)
		buff[i] = '\0';
	dup2(pipe_fd[0], STDIN_FILENO);
	status = read(pipe_fd[0], buff, 100);
	if (status == -1)
		return ;
	this->getRp()->setBody(buff, "html");
	std::cout<< "buff : "<<this->getRp()->getBody();
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

char	**CGI::create_env()
{
	char **env = new char *[_rq.getHeader().size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = _rq.getHeader().begin(); it != _rq.getHeader().end(); it++)
	{
		std::string tmp;
		tmp.append(it->first + "=" + it->second);
		env[i] = strdup(tmp.c_str());
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	**CGI::create_av()
{
	char **av;

	av = (char **) malloc(sizeof(char *) * 2);
	if (!av)
		return (NULL);
	std::string name = _rq.getRql().getUrl().getPath();
	av[0] = strdup(&(name.c_str())[1]);
	av[1] = NULL;
	return (av);
}
