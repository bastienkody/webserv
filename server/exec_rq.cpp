/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_rq.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:31:57 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/14 17:22:34 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../include/CGI.hpp"
#include "../include/Exception.hpp"

/*ajouter Location obj pour check methods allows*/

Response	exec_rq(Request rq, ConfigFile config)
{
	Response rp;
	std::string tmp;
	std::string path = rq.getRql().getUrl().getPath();

	(void) config;
	tmp = &path[path.size() - 3];
	try{
		if (tmp == ".py")
				CGI cgi(&rp, rq);
		else if (path[path.size() - 1] == '/')
			(void) tmp;// rq_dir(&rp, rq);
		else
			rq_html(&rp, rq);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return (rp);
}

Response	exec_rq_error(__attribute__((unused))Request rq, __attribute__((unused))ConfigFile config, int code)
{
	Response		rp;

	// create status line
	rp.setLineState(code);
	// create header
	rp.setHeader(rq);
	
	// check if error page exists in config file to fulfill body; if not no body?
	
	return (rp);
}
// int main(void)
// {
// 	std::string request = "GET http://localhost:80/home.txt?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\nformat: text\n";

// 	Request rq(request);
// 	rq.print();
// 	exec_rq(rq);
// }
