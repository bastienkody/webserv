/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:48:42 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/14 23:12:00 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bastien/RequestParsing/Request.hpp"
#include "../include/Server.hpp"


/*creer check dir -> return 0 si pas un dir*/

int	check_file(Request rq, std::string dir, int mode)
{
	std::string path = dir + rq.getRql().getUrl().getPath();
	if (access(path.c_str(), F_OK) == -1)
		return (1);
	if (mode == 1 && access(path.c_str(), R_OK) == -1)
		return (2);
	if (mode == 2 && access(path.c_str(), X_OK) == -1)
		return (2);
	return (0);
}