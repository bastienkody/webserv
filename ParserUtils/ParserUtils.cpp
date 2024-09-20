#include"ParserUtils.hpp"
#include <cctype>

//	rm OWS (l and r)
std::string	ParserUtils::trimOWS(std::string s)
{
	std::string::iterator	it = s.begin();
	while(it != s.end() && isspace(*it))
		++it;
	s.erase(s.begin(), it);

	std::string::reverse_iterator rit = s.rbegin();
	while(rit != s.rend() && isspace(*rit))
		++rit;
	if (rit.base() != s.end())
		s.erase(rit.base(), s.end());
	return s;
}

//	rm all OWS
std::string	ParserUtils::eraseOWS(std::string s)
{
	std::string				res;
	std::string::iterator	it = s.begin();
	while(it != s.end())
	{
		if(!isspace(*it))
			res.append(1, *it);
		++it;
	}
	return res;
}

// must add a eraseAfterDelim(std::string s, std::string delim, bool delimRm) et eraseBefore (bool pour savoir si rm le delim aussi)
//	rm everything after a #
std::string	ParserUtils::eraseSufixComments(std::string s)
{
	if (s.find('#') == std::string::npos)
		return s;
	return s.erase(s.find('#'), s.size());
}

//	find the first WS of a string. returns its index, -1 if not
int	ParserUtils::firstWsPos(std::string line)
{
	int			ows_pos = 0;
	std::string::iterator	it=line.begin(), ite = line.end();
	while (isspace(*it) == 0 && it != ite)
	{
		++it;
		++ows_pos;
	}
	return it==ite ? -1 : ows_pos;
}

//	check terminating semicolon and removes it
bool	ParserUtils::checkRmSemiColon(std::string & line)
{
	if (line[line.size()-1] != ';')
		return false;
	line.erase(line.size() -1, 1);
	return true;
}

//	only digit? (no sign accepted)
bool	ParserUtils::isStrDigit(const std::string & line)
{
	for (std::string::const_iterator it = line.begin(); it != line.end(); ++it)
		if (isdigit(*it) == false)
			return false;
	return true;
}

//	only alpha?
bool	ParserUtils::isStrAlpha(const std::string & line)
{
	for (std::string::const_iterator it = line.begin(); it != line.end(); ++it)
		if (isalpha(*it) == false)
			return false;
	return true;
}

//	yc LF
bool	ParserUtils::isStrPrint(const std::string & line)
{
	for (std::string::const_iterator it = line.begin(); it != line.end(); ++it)
		if (isprint(*it) == false && *it != '\n')
			return false;
	return true;
}

bool	ParserUtils::compCaseInsensitive(const std::string & line, const std::string & ref)
{
	if (line.size() != ref.size())
		return false;
	for (unsigned int i = 0; i < line.size(); ++i)
		if (toupper(line[i]) != toupper(ref[i]))
			return false;
	return true;
}

//	returns occurences of c
unsigned int	ParserUtils::charCount(std::string & line, char c)
{
	unsigned int	nb = 0;
	for (std::string::iterator it = line.begin(); it != line.end(); ++it)
		if (*it == c)
			++nb;
	return nb;
}
