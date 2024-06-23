#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <iostream>

/*
	Test socketpair to communicate between server and cgi (pipes limit is around 15ko)
	Fd can be given as it is for pipe, or via envar
	use of read and write:
		for unknown size, char by char which can be super long
		if size is known: no pb for writing, and maybe we can also commuinicate it to cgi via envar to read once ?
*/

void	msgChildToParent(void)
{
	std::string	msg("salut a tous");
	int	pid, fd[2];

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0)
		throw std::runtime_error("socketpair failed");
	if ( (pid = fork()) < 0)
		throw std::runtime_error("fork failed");
	if (pid == 0)	//	child
	{
		close(fd[1]);
		write(fd[0], msg.c_str(), msg.size() + 1);
		close(fd[0]);
		return;
	}
	else	//	parent
	{
		std::string	str;
		char	buf;
		close(fd[0]);
		while (read(fd[1], &buf, 1) > 0) // char by char bc size unknown
			str += buf;
		close(fd[1]);
		std::cout << "parent has read:" << str << std::endl;
		wait(NULL);
	}
}

void	childExecveToParent(char **envp)
{
	int		pid, fd[2];
	char	binary[] = {"/usr/bin/echo"};
	char	arg[] = {"echo"};
	char	arg2[] = {"reponse html envoye par le cgi"};
	char	*av[] = {arg, arg2, NULL};

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0)
		throw std::runtime_error("socketpair failed");

	if ( (pid = fork()) < 0)
		throw std::runtime_error("fork failed");

	if (pid == 0)	//	child
	{
		close(fd[1]);	// close otherside of socketpair
		
		if (dup2(fd[0], 1) < 0)
		{
			std::cout << "dup failed" << std::endl;
			return;
		}
		close(fd[0]);
		execve(binary, av, envp);
		std::cout << "execve failed" << std::endl;
		return;
	}

	if (pid > 0)	//	parent
	{
		std::string	str;
		char		buf[1024];

		close(fd[0]);	// close otherside of socketpair
		while (memset(buf, 0, sizeof(buf)) && read(fd[1], buf, 1023) > 0) 
			str += buf;
		close(fd[1]);
		std::cout << str.size() << "char read; str:" << str << std::endl;
		wait(NULL);
	}
}

int	main(__attribute((unused))int ac, __attribute((unused))char **av, char **envp)
{
	//msgChildToParent();
	childExecveToParent(envp);
}

