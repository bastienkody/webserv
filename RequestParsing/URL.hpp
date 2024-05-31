#ifndef URL_HPP
#define URL_HPP

#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<vector>
#include<algorithm>

/*
origin form : 		/myfile.html (direct format)
vs absolute form :	http://domainname:80/myfile.html (via proxy format)
most prob no absolute form but in case ... we parse it
*/

class URL
{
	public:
		URL();
		~URL();
		URL(const std::string name);
		URL(const URL & src);
		URL & operator=(const URL & rhs);

		std::string		getFull() const;
		std::string		getProtocol() const;
		std::string		getAuthority() const;
		std::string		getPort() const;
		std::string		getPath() const;
		std::string		getQuery() const;
		std::string		getAnchor() const;

		void			printDebug() const;

	private:

		std::string		_fullURL;
		std::string		_protocol;
		std::string		_authority;
		std::string		_port; //(conv via sstream for ovlf/neg later!)
		std::string		_path;
		std::string		_query;
		std::string		_anchor;

		void	absoluteFormParser();
		void	originFormParser(std::string url);
};

std::ostream& operator<<(std::ostream& os, const URL &rhs);

#endif
