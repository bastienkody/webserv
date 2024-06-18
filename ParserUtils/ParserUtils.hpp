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
		static int			firstWsPos(std::string line);
		static bool			checkRmSemiColon(std::string & line);
		static bool			isStrDigit(std::string & line);
		static bool			isStrAlpha(std::string & line);
		static unsigned int	charCount(std::string & line, char c); // bc I dont want to include algorithm
};

#endif
