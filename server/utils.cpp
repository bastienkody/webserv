/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:48:42 by mmuesser          #+#    #+#             */
/*   Updated: 2024/09/05 17:39:40 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../RequestParsing/Request.hpp"
#include "../include/server.hpp"
#include "../ConfigFile/Server.hpp"


/*creer check dir -> return 0 si pas un dir*/

int	check_file(std::string path, int mode)
{
	if (access(path.c_str(), F_OK) == -1)
		return (1);
	if (mode == 1 && access(path.c_str(), R_OK) == -1)
		return (2);
	if (mode == 2 && access(path.c_str(), X_OK) == -1)
		return (2);
	return (0);
}

// mod dest_url to real URL (og, or redirected) and returns 0 if not redirected, else 301/302
int	is_url_redirected(const std::string og_url, std::string &dest_url, Server serv, int index_loc)
{
	std::map<std::string, struct rewrite> redirections = find_redirections(serv, index_loc);
	for (std::map<std::string,struct rewrite>::const_iterator it = redirections.begin(); it != redirections.end(); ++it)
	{
		if (og_url == it->first)
		{
			dest_url = it->second.redirect_url;
			return it->second.type;
		}
	}
	//dest_url = og_url;
	return 0;
}

// not case isensitive for header check
bool	is_rq_finished(std::string raw)
{
	// fin des headers ?
	if (raw.find("\r\n\r\n") == std::string::npos)
		return std::cerr << "is_rq_finish: not yet CRLF+CRLF" << std::endl, false;

	Request	rq;
	rq.appendRaw(raw.data(), raw.size());
	rq.parse();

	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();
	if (it == ite)
		std::cout << "is_rq_finish: NO header" << std::endl;
	for (; it!=ite; ++it)
	{
		std::cout <<"header:" << it->first << "->" << it->second << std::endl;
		if (ParserUtils::compCaseInsensitive(it->first, "Transfer-Encoding") && ParserUtils::compCaseInsensitive(it->second, "chunked"))
		{
			std::string end = raw.substr(raw.size() -5, raw.size());
			char a = end[0];
			char b = end[1];
			char c = end[2];
			char d = end[3];
			char e = end[4];
			if (end != "0\r\n\r\n")
			{
				std::cerr << "is_rq_finish: not last chunked, we have:"<< std::endl;
				std::cerr << std::dec << static_cast<int>(a)<<"-"<<static_cast<int>(b)<<"-"<<static_cast<int>(c)<<"-"<<static_cast<int>(d)<<"-"<<static_cast<int>(e)<<std::endl;
				return false;
			}
			else
				std::cerr << "is_rq_finish: end of chunked" << std::endl;
		}
		else if (ParserUtils::compCaseInsensitive(it->first, "Content-Length"))
		{
			std::stringstream	sstr(it->second);
			unsigned int		i;
			sstr >> i;
			if (i != rq.getBody().size())
				return std::cerr << "is_rq_finish: content_len("<<i<< ") != body.size("<<rq.getBody().size()<<")" << std::endl, false;

		}
	}
	return std::cerr << "is_rq_finish: YES" << std::endl, true;
}


// test absolute then non absolute + correct not perfect match
int find_location2(const std::string path, Server serv)
{
	size_t size = 0, index = -1;

	// absolute match
	for(size_t i = 0; i < serv.getLocations().size(); i++)
		if (serv.getLocations()[i].getIsPathAbsolute() == true && path == serv.getLocations()[i].getPath())
				return i;
	// non absolute : perfect or longest match
	for(size_t i = 0; i < serv.getLocations().size(); i++)
	{
		std::string loc_path = serv.getLocations()[i].getPath();
		if (serv.getLocations()[i].getIsPathAbsolute() == true || loc_path.size() > path.size())
			continue;
		if (path == loc_path)
			return i;
		for (size_t x = 0; x < loc_path.size() && path[x] == loc_path[x]; x++)
		{
			if (x == loc_path.size() - 1 && x >= size)
			{
				size = x;
				index = i;
			}
		}
	}
	return (index);
}

std::string concatenate_root_path(Request rq, ConfigFile config, int index_serv, int index_loc)
{
	std::string root = find_str_data(config.getServers()[index_serv], index_loc, "root");
	if (root.size() == 0)
		return (std::string());
	std::string path = root + rq.getRql().getUrl().getPath();
	std::string loc_path = config.getServers()[index_serv].getLocations()[index_loc].getPath();
	path.erase(path.find(loc_path), loc_path.size());
	return (path);
}

/*A modifier ?*/
// int find_location(const std::string path, Server serv)
// {
// 	size_t size = 0;
// 	int index = -1;
// 	for(size_t i = 0; i < serv.getLocations().size(); i++)
// 	{
// 		if (path == serv.getLocations()[i].getPath())
// 		{
// 			if (serv.getLocations()[i].getIsPathAbsolute() == true)
// 				return (i);
// 			else
// 			{
// 				for (size_t y = i; y < serv.getLocations().size(); y++)
// 				{
// 					if (path == serv.getLocations()[i].getPath()
// 						&& serv.getLocations()[i].getIsPathAbsolute() == true)
// 						return (y);
// 				}	
// 				return (i);
// 			}
// 		}
// 		if (serv.getLocations()[i].getIsPathAbsolute() == false)
// 		{
// 			for (size_t y = 0; y < serv.getLocations()[i].getPath().size(); y++)
// 			{
// 				if (serv.getLocations()[i].getPath()[y] != path[y] && y > size)
// 				{
// 					index = i;
// 					size = y;
// 				}
// 			}
// 		}
// 	}
// 	return (index);
// }

std::string	find_str_data(Server serv, int index_loc, std::string to_find)
{
	std::string lst_data[4] = {"root", "index", "auto_index", "body_size"};
	for (int i = 0; i < 4; i++)
	{
		if (to_find == lst_data[i])
		{
			switch (i)
			{
				case 0:
					if (index_loc > -1 && serv.getLocations()[index_loc].getRoot().size() != 0)
					{
						return (serv.getLocations()[index_loc].getRoot());
					}
					else if (serv.getRoot().size() != 0)
						return (serv.getRoot());
					break ;
				case 1:
					if (index_loc > -1 && serv.getLocations()[index_loc].getIndex().size() != 0)
						return (serv.getLocations()[index_loc].getIndex());
					else if (serv.getIndex().size() != 0)
						return (serv.getIndex());
					break ;
				case 2:
					if (index_loc > -1 && serv.getLocations()[index_loc].getAutoIndex().size() != 0)
						return (serv.getLocations()[index_loc].getAutoIndex());
					else if (serv.getAutoIndex().size() != 0)
						return (serv.getAutoIndex());
					break ;
				case 3:
					if (index_loc > -1 && serv.getLocations()[index_loc].getMaxBodySize().size() != 0)
						return (serv.getLocations()[index_loc].getMaxBodySize());
					else if (serv.getMaxBodySize().size() != 0)
						return (serv.getMaxBodySize());
					break ;
			}
		}
	}
	return (std::string());
}

std::map<std::string, struct rewrite> find_redirections(Server serv, int index_loc)
{
	if (index_loc > -1 && serv.getLocations()[index_loc].getRedirection().size() != 0)
		return (serv.getLocations()[index_loc].getRedirection());
	else if (serv.getRedirection().size() != 0)
		return (serv.getRedirection());
	return (std::map<std::string, struct rewrite>());
}

std::map<std::string, std::string> find_error_pages(Server serv, int index_loc)
{
	if (index_loc > -1 && serv.getLocations()[index_loc].getErrorPages().size() != 0)
		return (serv.getLocations()[index_loc].getErrorPages());
	else if (serv.getErrorPages().size() != 0)
		return (serv.getErrorPages());
	return (std::map<std::string, std::string>());
}

std::vector<std::string> find_vector_data(Server serv, int index_loc, std::string to_find)
{
	std::string lst_data[3] = {"allow_methods", "cgi_ext", "cgi_pathes"};
	for (int i = 0; i < 3; i++)
	{
		if (to_find == lst_data[i])
		{
			switch (i)
			{
				case 0:
					if (index_loc > -1 && serv.getLocations()[index_loc].getAllowMethods().size() != 0)
						return (serv.getLocations()[index_loc].getAllowMethods());
					else if (serv.getAllowMethods().size() != 0)
						return (serv.getAllowMethods());
					break ;
				case 1:
					if (index_loc > -1 && serv.getLocations()[index_loc].getCgiExt().size() != 0)
						return (serv.getLocations()[index_loc].getCgiExt());
					else if (serv.getCgiExt().size() != 0)
						return (serv.getCgiExt());
					break ;
				case 2:
					if (index_loc > -1 && serv.getLocations()[index_loc].getCgiPathes().size() != 0)
						return (serv.getLocations()[index_loc].getCgiPathes());
					else if (serv.getCgiPathes().size() != 0)
						return (serv.getCgiPathes());
					break ;
					
			}
		}
	}
	return (std::vector<std::string>());
}
