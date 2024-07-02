#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <iostream>

/*
	Test socketpair to communicate between server and cgi (pipes limit is around 15ko)
*/

// child send msg to parent, no execve
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

// child execve command (no stdin capabilities), write to stdout dup on socket, parent read ohterside of socket
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
			return std::cout << "dup failed" << std::endl, (void)0;
		close(fd[0]);
		execve(binary, av, envp);
		return std::cout << "execve failed" << std::endl, void(0);
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

// try to execve child with both stdin and stdout on socket --> works! but cant send eof to cat; maybe its ok
// cgi would use a kinda read? with ls command parent reads EOF (read returns 0)
void	childExecveStdinStdout(char **envp)
{
	int		pid, fd[2];
	char	binary[] = {"/usr/bin/cat"};
	char	arg[] = {"cat"};
	char	arg2[] = {"-e"};
	char	*av[] = {arg, arg2, NULL};

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0)
		throw std::runtime_error("socketpair failed");

	if ( (pid = fork()) < 0)
		throw std::runtime_error("fork failed");
	if (pid == 0)	//	child
	{
		//std::cout << "child before duping" << std::endl;
		close(fd[1]);
		if (dup2(fd[0], 1) < 0)
			return std::cout << "dup stdout failed" << std::endl, (void)0;
		if (dup2(fd[0], 0) < 0)
			return std::cout << "dup stdin failed" << std::endl, (void)0;
		close(fd[0]);
		execve(binary, av, envp);
		return std::cout << "execve failed" << std::endl, void(0);
	}
	if (pid > 0)	//	parent
	{
		std::string	str;
		char		buf[1024];
		int			rd;

		close(fd[0]);
		write(fd[1], "la requete\n", strlen("la requete\n"));
		//std::cout << "parent before reading" << std::endl;
		while (memset(buf, 0, sizeof(buf)) && (rd = read(fd[1], buf, 1023)) > 0)
		{
			std::cout << "parent has read:" << rd << "bytes into buf :" << buf  << std::endl;
			str += buf;
		}
		close(fd[1]);
		std::cout << str.size() << "char read; str:" << str << std::endl;
		wait(NULL);
	}
}

int	main(__attribute((unused))int ac, __attribute((unused))char **av, char **envp)
{
	//msgChildToParent();
	//childExecveToParent(envp);
	childExecveStdinStdout(envp);
}

