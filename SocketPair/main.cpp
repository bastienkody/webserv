#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <iostream>

/*
	Test socketpair to communicate between server and cgi (pipes limit is around 15ko)
	use of read and write with unknown size --> buffer[1024] avec du memset avant chaque read
	socket fd is forward to cgi child by a dup on stdout
	I cant make it work "biderictionnal" with a dup for stdin + stdout on the same socketfd in cgi child 
		id like to send the rq to cgi stdin (writint it in father socketfd) and then just read the socketfd in father but it doesnot work
		it should be possible since socket are biderectionnal
		maybe a read/write pb only (trying to write before reading/flushing the socketpair?)
		it would work with two socketpair (one to send data to cgi, one to get get data from cgi) but ewwww
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
	char	binary[] = {"/usr/bin/ls"};
	char	arg[] = {"ls"};
	char	arg2[] = {"-la"};
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

