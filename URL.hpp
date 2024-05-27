#ifndef URL_HPP
#define URL_HPP

#include<iostream>
#include<string>
#include<map>
#include <vector>
#include<algorithm>

/*
origin form : 		/myfile.html (direct format)
vs absolute form :	http://domainname:80/myfile.html (via proxy format)
most prob no absolute form but in case ... we parse it
*/

class URL
{
	public:
		~URL();
		URL(const std::string name);
		URL(const URL & src);
		URL & operator=(const URL & rhs);

		std::string							getFull() const;
		std::string							getProtocol() const;
		std::string							getAuthority() const;
		unsigned int						getPort() const;
		std::string							getPath() const;
		std::map<std::string, std::string>	getQueries() const;
		std::string							getAnchor() const;

	private:
		URL();

		std::string							_fullURL;
		std::string							_protocol;
		std::string							_authority;
		unsigned int						_port; //(conv via sstrean for ovlf/neg etc)
		std::string							_path;
		std::map<std::string, std::string>	_queries;
		std::string							_anchor;

		void	absoluteFromParser();
};

std::ostream& operator<<(std::ostream& os, const URL &rhs);

#endif
