/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:30:22 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/13 15:29:28 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

// #include "Server.hpp"
#include "../RequestParsing/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../Response/Response.hpp"
#include <errno.h>

class CGI
{
	public:
		CGI(void);
		~CGI(void);
		CGI(const CGI &obj);
		CGI & operator=(const CGI &obj);
		CGI(Response *rp, Request rq, ConfigFile config, int index_serv, int index_loc);

		char		**create_env();
		char		**create_av();
		void		  exec_son(int *pipe_fd, std::string path, char **env, char **av);
		void		  exec_father(int *pipe_fd, std::string path, int pid);
		void		  init_env();


		Response							*getRp() const;
		Request		 						 getRq() const;
		std::map<std::string, std::string>	 getEnv() const;
	private:
		Response							*_rp;
		Request								 _rq;
		ConfigFile 							 _config;
		std::map<std::string, std::string>	 _env;
		int									 _index_serv;
		int									 _index_loc;
};

#endif
