/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:50:31 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/11 18:44:51 by mmuesser         ###   ########.fr       */
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
std::map<std::string, std::string> CGI::getEnv()	const {return _env;}

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
		this->exec_father(pipe_fd, path, status);
}

void	CGI::exec_son(int *pipe_fd, std::string path)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	
	char **env = create_env();
	char **av = create_av();
	/*check premiere ligne script ???*/
	execve(path.c_str(), av, env);
	delete [] env;
	delete [] av;
	perror("Execve");
	std::cerr<< "blabla test"<<std::endl;
	exit(-1);
}

/*definir limite pour reponse body*/
void	CGI::exec_father(int *pipe_fd, std::string path, int pid)
{
	write(pipe_fd[1], _rq.getBody().c_str(), _rq.getBody().size());
	int status;
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == -1)
	{
		std::cerr<< "qwertyuiop"<<std::endl;
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		*_rp = exec_rq_error(_rq, _config, 500, _index_serv, _index_loc);
		return ;
	}
	// (void) path;
	char *buff;
	buff = (char *) malloc(sizeof(char) * (4095 + 1));
	if (!buff)
		return ;
	for (int i = 0; i < 4095; i++)
		buff[i] = '\0';
	dup2(pipe_fd[0], STDIN_FILENO);
	std::cerr<< "fohsoefhs[oefhsef]"<<std::endl;
	status = read(pipe_fd[0], buff, 4095);
	if (status == -1)
		return ;
	this->getRp()->setLineState(201);
	this->getRp()->setHeader(_rq, _config, _index_serv, _index_loc);
	this->getRp()->setLocation(path);
	this->getRp()->setBody(buff, "html");
	free(buff);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	CGI::init_env()
{
	_env["REQUEST_METHOD"] = _rq.getRql().getVerb();
	if (_rq.getRql().getVerb() == "POST")
	{
		_env["CONTENT_TYPE"] = _rq.getHeader().find("Content-Type")->second;
		_env["CONTENT_LENGTH"] = _rq.getHeader().find("Content-Length")->second;
	}
	_env["QUERY_STRING"] = _rq.getRql().getUrl().getQuery();
}

char	**CGI::create_env()
{
	this->init_env();
	char **env = new char *[(_rq.getHeader().size() + _env.size() + 1)];
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = _rq.getHeader().begin(); it != _rq.getHeader().end(); it++)
	{
		std::string tmp;
		tmp.append(it->first + "=" + it->second);
		env[i] = strdup(tmp.c_str());
		i++;
	}
	for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); it++)
	{
		std::string tmp;
		tmp.append(it->first + "=" + it->second);
		env[i] = strdup(tmp.c_str());
		i++;
	}
	env[i] = NULL;
	
	std::cerr<< "env :"<<std::endl;
	i = 0;
	while (env[i] != NULL)
	{
		std::cerr<< "\t" << env[i]<<std::endl;
		i++;
	}
	std::cerr<< "\n";
	return (env);
}

char	**CGI::create_av()
{
	char **av;

	av = (char **) malloc(sizeof(char *) * 3);
	if (!av)
		return (NULL);
	std::string name = _rq.getRql().getUrl().getPath();
	std::string ext_path = find_vector_data(_config.getServers()[_index_serv], _index_loc, "cgi_pathes")[0];
	av[0] = strdup(ext_path.c_str());
	av[1] = strdup(&(name.c_str())[1]);
	av[2] = NULL;
	return (av);
}
