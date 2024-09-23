/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 14:46:02 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/20 00:20:29 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ConfigFile/ConfigFile.hpp"
#include "../include/server.hpp"
#include "../Clients/Clients.hpp"
#include "../ConfigFile/Server.hpp"

int create_socket_server(Server serv)
{
	struct sockaddr_in adress;
	int server_fd;
	int flag_true = 1;

	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = inet_addr(serv.getIp().c_str());
	adress.sin_port = htons(serv.getPort());

	// create the server socket
	if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (perror("socket"), -1);
	// set options
	fcntl(server_fd, F_SETFL, O_NONBLOCK); /*je vois pas encore de diff avec et sans*/
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag_true, sizeof(flag_true)) != 0)
		return (perror("setsockopt"), -1);
	// make it listen on port
	if (bind(server_fd, (struct sockaddr *) &adress, sizeof(adress)) < 0)
		return (perror("bind"), close(server_fd), -1);
	if (listen(server_fd, 20) < 0)
		return (perror("listen"), close(server_fd), -1);
	std::cout<< "[Server] New server socket created and listening to port " << serv.getPortSTR().c_str()<<std::endl;
	return (server_fd);
}

size_t read_recv_data(int i, Poll *poll_fds, struct client &co)
{
	int nb_bytes;
	char buff [4096];
	
	memset(&buff, 0, sizeof(buff));
	nb_bytes = recv(poll_fds->getFds(i).fd, &buff, 4095, 0);
	if (nb_bytes <= 0)
	{
		if (nb_bytes < 0)
			perror("recv");
		else
			if (DEBUGP) {std::cout<< "[Server] Connexion with " << poll_fds->getFds(i).fd << " is closed."<<std::endl;}
	}
	else
	{
		co.rq.appendRaw(buff, nb_bytes);
		//if (DEBUGP) {std::cout<< "[Client "<< poll_fds->getFds(i).fd<< "] " << buff  << std::endl;}
	}
	return nb_bytes;
}

int	send_response(struct client &co, ConfigFile config)
{
	co.rq.parse();
	//co.rq.print();

	int	code;
	int	index_serv = config.getServerFromFd(co.server_fd);
	int	index_loc = find_location(co.rq.getRql().getUrl().getPath(), config.getServers()[index_serv]);

	//std::cout << "servnb:" << index_serv << ", locnb:" << index_loc << std::endl;

	if ((code = RequestChecking::CheckBasics(co.rq, config.getServers()[index_serv])) != 0)
		co.rp = exec_rq_error(co.rq, config, code, index_serv, index_loc);
	else if (co.rq.getRql().getVerb().compare("POST") == 0 && (code = RequestChecking::CheckRequiredHeaderPOST(co.rq, find_str_data(config.getServers()[index_serv], index_loc, "body_size"))) != 1)
	{
		if (code == 413 || code == 400)
		{
			if (DEBUGP) {std::cout << "header post error" << std::endl;}
			co.rp = exec_rq_error(co.rq, config, code, index_serv, index_loc);
		}
		if (code == 2)
		{
			if (co.rq.unchunkBody() == false) // bad or missing sizing sequence in a chunk
				co.rp = exec_rq_error(co.rq, config, 400, index_serv, index_loc);
			else
				co.rp = exec_rq(co.rq, config, index_serv, index_loc);
		}
	}
	else
		co.rp = exec_rq(co.rq, config, index_serv, index_loc);

	//std::cout<< "CO.RP:\n" << co.rp.getWholeResponse()<< "\nEND CO.RP" << "bodysize:" << co.rp.getBody().size() <<std::endl;
	std::cout << "responding fd:" << co.fd << "(path:" << co.rq.getRql().getUrl() << ')' << std::endl << "#############################################################################" << std::endl;
	return send(co.fd, co.rp.getWholeResponse().c_str(), co.rp.getWholeResponse().size(), 0) <= 0 ? perror("send"), -1 : 1;// si erreur de send => virer le client sans re essayer de lui repondre.
}

/* Fonction qui tej les clients a appeler si count>255 */
int	accept_new_connection(int server_fd, Poll *poll_fds)
{
	int client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0)
		return (std::cerr<<"serverfd:"<<server_fd<<std::endl,perror("accept"), -1);
	if (poll_fds->getCount() > 255)
		return (std::cerr<<"poll_fds._count>255 in accept new connection"<<std::endl, close(client_fd), -1);
	poll_fds->add_to_poll(client_fd);
	if (DEBUGP) {std::cout<< "[Server] New connexion with client fd : " << poll_fds->getFds(poll_fds->getCount() - 1).fd<<std::endl;}
	return client_fd;
}

/*verifie quelle socket server a recu une nouvelle connexion*/
int	check_serv_socket(int fd, unsigned int *serv_fds, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if ((unsigned int) fd == serv_fds[i])
			return (i);
	}
	return (-1);
}
