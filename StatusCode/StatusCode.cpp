#include"StatusCode.hpp"

//	Default destructor
StatusCode::~StatusCode() {}

//	Default constructor
StatusCode::StatusCode()
{
	std::string whole("100 Continue\n\
101 Switching Protocols\n\
102 Processing\n\
103-199 Unassigned\n\
200 OK\n\
201 Created\n\
202 Accepted\n\
203 Non-Authoritative Information\n\
204 No Content\n\
205 Reset Content\n\
206 Partial Content\n\
207 Multi-Status\n\
208 Already Reported\n\
209-225 Unassigned\n\
226 IM Used\n\
227-299 Unassigned\n\
300 Multiple Choices\n\
301 Moved Permanently\n\
302 Found\n\
303 See Other\n\
304 Not Modified\n\
305 Use Proxy\n\
306 Reserved\n\
307 Temporary Redirect\n\
308 Permanent Redirect\n\
309-399 Unassigned\n\
400 Bad Request\n\
401 Unauthorized\n\
402 Payment Required\n\
403 Forbidden\n\
404 Not Found\n\
405 Method Not Allowed\n\
406 Not Acceptable\n\
407 Proxy Authentication Required\n\
408 Request Timeout\n\
409 Conflict\n\
410 Gone\n\
411 Length Required\n\
412 Precondition Failed\n\
413 Request Entity Too Large\n\
414 Request-URI Too Long\n\
415 Unsupported Media Type\n\
416 Requested Range Not Satisfiable\n\
417 Expectation Failed\n\
422 Unprocessable Entity\n\
423 Locked\n\
424 Failed Dependency\n\
425 Unassigned\n\
426 Upgrade Required\n\
427 Unassigned\n\
428 Precondition Required\n\
429 Too Many Requests\n\
430 Unassigned\n\
431 Request Header Fields Too Large\n\
432-499 Unassigned\n\
500 Internal Server Error\n\
501 Not Implemented\n\
502 Bad Gateway\n\
503 Service Unavailable\n\
504 Gateway Timeout\n\
505 HTTP Version Not Supported\n\
506 Variant Also Negotiates (Experimental)\n\
507 Insufficient Storage\n\
508 Loop Detected\n\
509 Unassigned\n\
510 Not Extended\n\
511 Network Authentication Required\n\
512-599 Unassigned\n");

	while(whole.find('\n') != std::string::npos)
	{
		std::string line (whole.substr(0, whole.find('\n')));
		whole.erase(0, whole.find('\n') + 1);

		int key = std::atoi(line.substr(0, line.find(' ')).c_str());
		std::string val(line.substr(line.find(' ') + 1, line.find("\n")));
		_data.insert(std::pair<int, std::string>(key, val));
	}
}


//	Getter
std::string const & StatusCode::getPhrase(int code) const
{
	return (_data.find(code)->second);
}

//	printer
void StatusCode::printAll() const
{
	std::map<int, std::string>::const_iterator it = _data.begin();
	std::map<int, std::string>::const_iterator ite = _data.end();

	for(; it!=ite; ++it)
		std::cout << it->first << ':' << it->second << std::endl;
}

/*int	main(void)
{
	StatusCode sp;

	//sp.printAll();
	std::cout << "501: " + sp.getPhrase(501) << std::endl;
	std::cout << "404: " + sp.getPhrase(404) << std::endl;
	std::cout << "200: " + sp.getPhrase(200) << std::endl;
}*/
