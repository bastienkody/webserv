/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:50:31 by mmuesser          #+#    #+#             */
/*   Updated: 2024/08/13 17:54:09 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"
#include "../ConfigFile/ConfigFile.hpp"
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

CGI::CGI(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc) : _rp(rp), _rq(rq), _config(config), _index_serv(index_serv), _index_loc(index_loc)
{
	int status;
	int pipe_fd[2];

	std::string path = concatenate_root_path(_rq, _config, _index_serv, _index_loc);
	if (path.size() == 0)
	{
		*_rp = exec_rq_error(_rq, _config, 500, _index_serv, _index_loc);
		return ;
	}	
	status = check_file(path, 2);
	if (status > 0)
	{
		*_rp = exec_rq_error(_rq, _config, 404, _index_serv, _index_loc);
		return ;
	}
	status = pipe(pipe_fd);
	if (status == -1)
	{
		*_rp = exec_rq_error(_rq, _config, 500, _index_serv, _index_loc);
		return ;
	}
	status = fork();
	if (status == -1)
	{
		*_rp = exec_rq_error(_rq, _config, 500, _index_serv, _index_loc);
		return ;
	}
	else if (status == 0)
		this->exec_son(pipe_fd, path);
	else
		this->exec_father(pipe_fd);
}

void	CGI::exec_son(int *pipe_fd, std::string path)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	
	char **env = create_env();
	char **av = create_av();
	execve(path.c_str(), av, env);
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
	this->getRp()->setLineState(200);
	this->getRp()->setHeader(_rq, _config, _index_serv, _index_loc);
	this->getRp()->setBody(buff, ".py");
	// std::cout<< "buff : "<<this->getRp()->getBody();
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
