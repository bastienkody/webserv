/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:30:22 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/28 18:29:36 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

// #include "Server.hpp"
#include "../RequestParsing/Request.hpp"
#include "../Response/Response.hpp"
#include "../include/Exception.hpp"
#include <errno.h>

class CGI
{
	public:
		CGI(void);
		~CGI(void);
		CGI(const CGI &obj);
		CGI & operator=(const CGI &obj);
		CGI(Response *rp, Request rq);

		char		**create_env();
		char		**create_av();
		void		  exec_son(int *pipe_fd);
		void		  exec_father(int *pipe_fd);

		Response	*getRp() const;
		Request		 getRq() const;
	private:
		Response	*_rp;
		Request		 _rq;
};

#endif
