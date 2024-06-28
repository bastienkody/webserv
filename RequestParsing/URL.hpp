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

		std::string		const & getFull() const;
		std::string		const & getProtocol() const;
		std::string		const & getAuthority() const;
		std::string		const & getPort() const;
		std::string		const & getPath() const;
		std::string		const & getQuery() const;
		std::multimap<std::string, std::string>	const & getQueries() const;
		std::string		const & getAnchor() const;

		void			printDebug() const;

	private:

		std::string		_fullURL;
		std::string		_protocol;
		std::string		_authority;
		std::string		_port; //(conv via sstream for ovlf/neg later!)
		std::string		_path;
		std::string		_query;
		std::multimap<std::string, std::string>	_queries;
		std::string		_anchor;

		void	absoluteFormParser();
		void	originFormParser(std::string url);
};

std::ostream& operator<<(std::ostream& os, const URL &rhs);

#endif