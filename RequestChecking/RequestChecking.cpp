#include"RequestChecking.hpp"
#include <algorithm>

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

//	no ws between key and colon --> 400 bad request (cf rfc9112 5.1)
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

//	looking for wether content_lenght or transfer-encoding
//	none : 0 (find the error code)
//	content_lenght : 1 (no chunk, its ok now)
//	chunked = 2 (on ignore un potentiel content-length, faut unchunk la rq)
//	content-lenght > max_body_size --> 413 Payload too large
int	RequestChecking::CheckRequiredHeaderPOST(const Request & rq, std::string max_body_size)
{
	bool		content_lenght = false;
	std::string	lenght = 0;
	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();

	for (; it!=ite; ++it)
	{
		if (it->first.compare("Transfer-Encoding") && it->second.find("chunked") != std::string::npos)
			return 2;
		if (it->first.compare("Content-Length"))
		{
			// cant return here if ever a chunk happens later, content_l must be ignored
			content_lenght = true;
			lenght = ParserUtils::trimOWS(it->second);
		}
	}
	if (content_lenght == false)
		return 0;
	if (max_body_size.size() == 0 || max_body_size.compare("-1")) // pas de maxbody en config : on est good
		return (1);

	std::stringstream	str(max_body_size), str2(lenght); // conv err with lenght (header content) possible
	unsigned int		i, j;
	str >> i;
	str2 >> j;
	return (i >= j ? 1 : 413);
}
