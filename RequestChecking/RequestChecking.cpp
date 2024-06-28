#include"RequestChecking.hpp"


int RequestChecking::CheckBasics(const Request & rq)
{
	if (rq.getRql().getUrl().getPath().size() == 0) // empty path in rql --> bad syntax
		return 400;
	if (rq.getRql().getVersion().compare(EXPECTED_VERSION) != 0) // version not supported --> 505
		return 505;
	std::string	verb(rq.getRql().getVerb());
	if (verb.compare("GET") && verb.compare("POST") && verb.compare("DELETE")) // method not implemented --> 501 (server connait que get, post and delete. 405 sera envoye si on connait la methode mais ne lautorise pas via configfile)
		return 401;
	return CheckHeaderKey(rq);
}

//	no ws between key and colon --> 400 bad request
//	no empty key --> 400 bad request 
//	no host --> 400 bad request (cs rfc 2616 14.23)
int	RequestChecking::CheckHeaderKey(const Request & rq)
{
	bool	isHostPresent = false;
	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();

	for (; it!=ite; ++it)
	{

		if (it->first.size() == 0 || isspace(it->first.back()))
			return 400;
		if (it->first.compare("Host") == 0)
			isHostPresent = true;
	}
	return (isHostPresent ? 0 : 400);
}
