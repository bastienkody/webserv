#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

#include<iostream>
#include<string>
#include "URL.hpp"

#define	EXPECTED_VERSION "HTTP/1.1"
#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"

class URL;

class RequestLine
{
	public:
		RequestLine();

		~RequestLine();
		RequestLine(const std::string name);
		RequestLine(const RequestLine & src);
		RequestLine & operator=(const RequestLine & rhs);

		std::string	const & getFull() const;
		std::string	const & getVerb() const;
		std::string	const & getVersion() const;
		bool		const & getIsVersionGood() const;
		bool		const & getIsVerbGood() const;
		URL			const & getUrl() const;

	private:
		
		std::string		_fullRequestLine;
		std::string		_verb;
		bool			_isVerbGood;
		std::string		_version;
		bool			_isVersionGood;
		URL				_Url;
		
};

#endif