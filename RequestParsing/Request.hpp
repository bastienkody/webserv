#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include <sys/socket.h>
#include "RequestLine.hpp"

class RequestLine;

/*
	ajouter un element RawData qui s'appendra si la req est lue en plusieurs read_recv
	Passez par un "appendData" a chaque read_recv (attention)
	Puis parser la requete a partir d'un setRq (vs le const parametric!)
*/

class Request
{
	public:
		Request();
		~Request();
		Request(const Request & src);
		Request & operator=(const Request & rhs);

		RequestLine				const & getRql() const;
		std::map<std::string, std::string>	const & getHeader() const;
		std::string				const & getBody() const;
		std::string				const & getRaw() const;

		void	parse();
		void	appendRaw(std::string data);
		void	unchunk(int fd);
		void	print() const;

	private:

		std::string				_raw;
		RequestLine				_rql;
		std::map<std::string, std::string>	_header;
		std::string				_body;
};

#endif
