/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:59 by mmuesser          #+#    #+#             */
/*   Updated: 2024/07/08 18:55:43 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile/ConfigFile.hpp"
#include "ConfigFile/Server.hpp"
#include "ConfigFile/Location.hpp"
#include "Clients/Clients.hpp"
#include "include/Poll.hpp"
#include "include/server.hpp"
#include <unistd.h>
#include <vector>

static std::string rep("HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 133\nContent-Type: text/html\nConnection: Keep-alive\n\n<html><head><title>Hello World - First CGI Program</title></head><h2>Hello World! This is my first CGI program</h2></body?</html>\n");

unsigned int *list_server_fd(Poll poll_fds)
{
	unsigned int *dest;

	dest = (unsigned int *) malloc(sizeof(unsigned int) * (poll_fds.getCount()));
	if (!dest)
		return (0);
	for (int i = 0; i < poll_fds.getCount(); i++)
	{	
		dest[i] = poll_fds.getFds(i).fd;
	}
	return (dest);
}

void	send_response(__attribute__((unused))struct client co)
{
	std::cout << "send response to fd " << co.fd << std::endl;
	co.rq.parse();
	co.rq.print();
	send(co.fd, rep.c_str(), rep.size(), 0);
}

void	launch_server(__attribute__((unused))ConfigFile config, Poll poll_fds)
{
	unsigned int *server_fd = list_server_fd(poll_fds);
	std::vector<struct client>	clients;

	while (true)
	{
		std::cout << clients.size() << std::endl;
		int	status = poll_fds.call_to_poll();
		if (status < 0)
			return (perror("poll"));
		else if (status == 0)
			continue;
		for(int i = 0; i < poll_fds.getCount(); i++)
		{
			if (poll_fds.getFds(i).revents & POLLIN)
			{
				// new client requesting the server
				if ((status = check_serv_socket(poll_fds.getFds(i).fd, server_fd, poll_fds.getCount())) != -1)
					accept_new_connection(server_fd[status], &poll_fds);
				// data to read from the client request
				else
				{
					std::string buff = read_recv_data(i, &poll_fds);
					if (buff == "error recv")
						return ; /*leaks pas encore gere*/
					else if (buff == "connection closed")
						continue ;
					int pos = find_co_by_fd_pos(clients, poll_fds.getFds(i).fd);
					if (pos == -1)
					{
						struct client cli;
						cli.fd = poll_fds.getFds(i).fd;
						cli.rq.appendRaw(buff);
						cli.answered = false;
						clients.push_back(cli);
					}
					else
						clients[pos].rq.appendRaw(buff);
					//std::cout << "rq:" << clients[pos].rq.getRaw() << std::endl;
				}
			}
			// responding
			else if (clients.size() > 0 && poll_fds.getFds(i).revents & POLLOUT) // pe direct checker de quelle connection on parle? avec un iterator (pour erase)
			{
				if (clients[find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)].answered == false)
				{
					// function(buff, &poll_fds, i, config);
					send_response(clients[find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)]);
					// std::badalloc + core dumped here ; sometimes with curl, always with firefox
					clients[find_co_by_fd_pos(clients, poll_fds.getFds(i).fd)].answered = true;
					clients.erase(find_co_by_fd_it(clients, poll_fds.getFds(i).fd));
				}
            }
		}
	}
	free(server_fd);
}

int	verif_host(ConfigFile config, int i)
{
	for (int y = 0; y < i; y++)
	{
		if (config.getServers()[y].getIp() == config.getServers()[i].getIp()
			&& config.getServers()[y].getPort() == config.getServers()[i].getPort())
			return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return std::cerr << "expected config file as single argument" << std::endl, 2;
	
	ConfigFile	config(av[1]);
	try{
		config.openReadFileToStr();
		config.readAllInfos();
	}
	catch(const std::exception& e){
		return std::cerr << e.what() << std::endl, 1;
	}
	Poll poll_fds;
	for (size_t i = 0; i < config.getServers().size(); i++)
	{
		if (verif_host(config, i) == 1)
			continue ;
		int fd = create_socket_server(config.getServers()[i].getPortSTR().c_str());
		if (fd == -1)
			return 1;
		try{
			poll_fds.add_to_poll(fd);
		}
		catch(const std::exception& e){
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	launch_server(config, poll_fds);
	return 0;
}