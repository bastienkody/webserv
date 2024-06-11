#ifndef PARSERUTILS_HPP
#define PARSERUTILS_HPP

#include<iostream>
#include<string>

class ParserUtils
{
	public:
		static std::string	trimOWS(std::string s);
		static std::string	eraseOWS(std::string s);
		static std::string	eraseSufixComments(std::string s);
};


#endif
