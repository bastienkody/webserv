/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:31:43 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/05 18:30:01 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poll.hpp"
#include <unistd.h>
#include <sys/types.h>

/*
recuperer Request object
check si j'ai tout body via content_length (en checkant _header dans Request obj)
creer pipe et dup fd pour que resultat script soit recuperable vu qu'il va ecrire sur stdin
check verb (voir url selon les scripts qu'on aura) pour exec script correspondant
fork puis launch execve
*/

void	cgi(char **av, char **env)
{
	
}

int	main(int ac, char **av, char **env)
{
	cgi(av, env);
	return (0);
}