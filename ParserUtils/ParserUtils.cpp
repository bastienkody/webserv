#include"ParserUtils.hpp"

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

std::string	ParserUtils::eraseSufixComments(std::string s)
{
	if (s.find('#') == std::string::npos)
		return s;
	return s.erase(s.find('#'), s.size());
}
