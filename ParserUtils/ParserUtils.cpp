#include"ParserUtils.hpp"

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
	if (it == ite)
		return -1;
	return ows_pos;
}

bool	ParserUtils::checkRmSemiColon(std::string & line)
{
	if (line[line.size()-1] != ';')
		return false;
	line.erase(line.size() -1, 1);
	return true;
}
