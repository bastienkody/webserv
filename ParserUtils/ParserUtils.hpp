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
		static bool			isStrDigit(const std::string & line);
		static bool			isStrAlpha(const std::string & line);
		static bool			isStrPrint(const std::string & line);
		static bool			compCaseInsensitive(const std::string & line, const std::string & ref);
		static unsigned int	charCount(std::string & line, char c); // bc I dont want to include algorithm
};

#endif
