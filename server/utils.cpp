/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:48:42 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/28 18:42:40 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../RequestParsing/Request.hpp"
#include "../include/server.hpp"


/*creer check dir -> return 0 si pas un dir*/

char	**create_av(Request rq)
{
	char **av;

	std::string name = rq.getRql().getUrl().getPath();
	av[0] = strdup(&(name.c_str())[1]);
	av[1] = NULL;
	return (av);
}

char	**create_env(Request rq)
{
	char **env = new char *[rq.getHeader().size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = rq.getHeader().begin(); it != rq.getHeader().end(); it++)
	{
		std::string tmp;
		tmp.append(it->first + "=" + it->second);
		env[i] = strdup(tmp.c_str());
		i++;
	}
	env[i] = NULL;
	return (env);
}

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