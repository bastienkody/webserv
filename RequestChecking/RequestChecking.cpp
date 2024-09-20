#include"RequestChecking.hpp"
#include"../include/server.hpp"

int RequestChecking::CheckBasics(const Request & rq, const Server & serv)
{
	if (rq.getRql().getUrl().getPath().size() == 0) // empty path in rql --> bad syntax
		return 400;
	if (rq.getRql().getVersion().compare(EXPECTED_VERSION) != 0) // version not supported --> 505
		return 505;
	std::string	verb(rq.getRql().getVerb());
	if (verb.compare("GET") && verb.compare("POST") && verb.compare("DELETE")) // method not implemented --> 501 (server connait que get, post and delete. 405 sera envoye si on connait la methode mais ne lautorise pas via configfile)
		return 501;
	return CheckHeaderKey(rq, serv);
}

//	no ws between key and colon --> 400 bad request (cf rfc9112 5.1)
//	no empty key --> 400 bad request 
//	no host --> 400 bad request (cs rfc 2616 14.23)
int	RequestChecking::CheckHeaderKey(const Request & rq, const Server & serv)
{
	int	Host_nb = 0;
	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();

	for (; it!=ite; ++it)
	{
		if (it->first.size() == 0 || isspace(it->first[it->first.size() - 1]))
		{
			if(DEBUGP){std::cerr<<"headerfield: empty or space found"<<std::endl;}
			return 400;
		}
		if (ParserUtils::compCaseInsensitive(it->first, "Host"))
		{
			++Host_nb;
			if (CheckHost(it->second, serv) == false)
				return 400;
		}
	}
	if(DEBUGP && Host_nb!=1){std::cerr<<"Not exactly one host header"<<std::endl;}
	return (Host_nb == 1 ? 0 : 400);
}

/*	RFC 7230, 5.4:
A server MUST respond with a 400 (Bad Request) status code to any
HTTP/1.1 request message that lacks a Host header field and to any
request message that contains more than one Host header field or a
Host header field with an invalid field-value
port is optionnal
*/
bool	RequestChecking::CheckHost(const std::string & field, const Server & serv)
{
	if (field == serv.getIp() || field == std::string(serv.getIp() + ':' + serv.getPortSTR()))
		return true;
	for (std::vector<std::string>::const_iterator it = serv.getNames().begin(); it != serv.getNames().end(); ++it)
		if (ParserUtils::compCaseInsensitive(field, *it) == true || ParserUtils::compCaseInsensitive(field, *it + ':' + serv.getPortSTR()))
			return true;
	if(DEBUGP){std::cerr<<"Invalid host field"<<std::endl;}
	return false;
}

//	looking for wether content_lenght or transfer-encoding
//	none : 0 (or error code?)
//	content_lenght : 1
//	chunked = 2 (on ignore un potentiel content-length)
//	content-lenght > max_body_size --> 413 Payload too large
int	RequestChecking::CheckRequiredHeaderPOST(const Request & rq, std::string max_body_size)
{
	bool		content_lenght = false;
	std::string	lenght = "0";
	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();

	//std::cout << "REAL BODY SIZE: " << rq.getBody().size() << std::endl;

	for (; it!=ite; ++it)
	{
		if (ParserUtils::compCaseInsensitive(it->first, "Transfer-Encoding") && ParserUtils::compCaseInsensitive(it->second, "chunked"))
			return 2;
		if (ParserUtils::compCaseInsensitive(it->first, "Content-Length"))
		{
			content_lenght = true;
			lenght = ParserUtils::trimOWS(it->second);
		}
	}
	if (content_lenght == false)
		return 400;

	std::stringstream	str(max_body_size), str2(lenght);
	unsigned int		i, j;
	str >> i;
	str2 >> j;

	if (j != rq.getBody().size())
	{
		if(DEBUGP){std::cerr << "content_len!=body.size()" << std::endl;}
		return 400;
	}
	if (max_body_size.size() == 0)
		return 1;
	return (i >= j ? 1 : 413);
}

bool	RequestChecking::isKeepAlive(const Request & rq)
{
	std::multimap<std::string, std::string>::const_iterator it = rq.getHeader().begin();
	std::multimap<std::string, std::string>::const_iterator ite = rq.getHeader().end();

	for (; it!=ite; ++it)
		if (ParserUtils::compCaseInsensitive(it->first, "Connection") && ParserUtils::compCaseInsensitive(it->second, "Keep-Alive"))
			return true;
	return false;
}
