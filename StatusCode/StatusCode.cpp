#include"StatusCode.hpp"

const std::map<int, std::string> StatusCode::_data = StatusCode::initMap();

std::map<int, std::string> StatusCode::initMap(void)
{
    std::map<int, std::string> m;
    m[100] = "Continue";
	m[101] = "Switching Protocols";
	m[102] = "Processing";
	m[200] = "OK";
	m[201] = "Created";
	m[202] = "Accepted";
	m[203] = "Non-Authoritative Information";
	m[204] = "No Content";
	m[205] = "Reset Content";
	m[206] = "Partial Content";
	m[207] = "Multi-Status";
	m[208] = "Already Reported";
	m[300] = "Multiple Choices";
	m[301] = "Moved Permanently";
	m[302] = "Found";
	m[303] = "See Other";
	m[304] = "Not Modified";
	m[305] = "Use Proxy";
	m[306] = "Reserved";
	m[307] = "Temporary Redirect";
	m[308] = "Permanent Redirect";
	m[400] = "Bad Request";
	m[401] = "Unauthorized";
	m[402] = "Payment Required";
	m[403] = "Forbidden";
	m[404] = "Not Found";
	m[405] = "Method Not Allowed";
	m[406] = "Not Acceptable";
	m[407] = "Proxy Authentication Required";
	m[408] = "Request Timeout";
	m[409] = "Conflict";
	m[410] = "Gone";
	m[411] = "Length Required";
	m[412] = "Precondition Failed";
	m[413] = "Payload Too Large";
	m[414] = "Request-URI Too Long";
	m[415] = "Unsupported Media Type";
	m[416] = "Requested Range Not Satisfiable";
	m[417] = "Expectation Failed";
	m[422] = "Unprocessable Entity";
	m[423] = "Locked";
	m[424] = "Failed Dependency";
	m[425] = "Unassigned";
	m[426] = "Upgrade Required";
	m[428] = "Precondition Required";
	m[429] = "Too Many Requests";
	m[431] = "Request Header Fields Too Large";
	m[500] = "Internal Server Error";
	m[501] = "Not Implemented";
	m[502] = "Bad Gateway";
	m[503] = "Service Unavailable";
	m[504] = "Gateway Timeout";
	m[505] = "HTTP Version Not Supported";
	m[507] = "Insufficient Storage";
	m[508] = "Loop Detected";
	m[510] = "Not Extended";
	m[511] = "Network Authentication Required";
	return m;
}

//	Getter
std::string const & StatusCode::getPhrase(int code)
{
	return (_data.find(code)->second);
}

//	printer
void StatusCode::printAll()
{
	std::map<int, std::string>::const_iterator it = _data.begin();
	std::map<int, std::string>::const_iterator ite = _data.end();

	for(; it!=ite; ++it)
		std::cout << it->first << ':' << it->second << std::endl;
}

/*int	main(void)
{
	StatusCode::printAll();
}*/
