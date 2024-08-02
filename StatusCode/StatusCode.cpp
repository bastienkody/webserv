#include"StatusCode.hpp"

const std::map<int, std::string> StatusCode::_data = {
{100, "Continue"},
{101, "Switching Protocols"},
{102, "Processing"},
{200, "OK"},
{201, "Created"},
{202, "Accepted"},
{203, "Non-Authoritative Information"},
{204, "No Content"},
{205, "Reset Content"},
{206, "Partial Content"},
{207, "Multi-Status"},
{208, "Already Reported"},
{300, "Multiple Choices"},
{301, "Moved Permanently"},
{302, "Found"},
{303, "See Other"},
{304, "Not Modified"},
{305, "Use Proxy"},
{306, "Reserved"},
{307, "Temporary Redirect"},
{308, "Permanent Redirect"},
{400, "Bad Request"},
{401, "Unauthorized"},
{402, "Payment Required"},
{403, "Forbidden"},
{404, "Not Found"},
{405, "Method Not Allowed"},
{406, "Not Acceptable"},
{407, "Proxy Authentication Required"},
{408, "Request Timeout"},
{409, "Conflict"},
{410, "Gone"},
{411, "Length Required"},
{412, "Precondition Failed"},
{413, "Request Entity Too Large"},
{414, "Request-URI Too Long"},
{415, "Unsupported Media Type"},
{416, "Requested Range Not Satisfiable"},
{417, "Expectation Failed"},
{422, "Unprocessable Entity"},
{423, "Locked"},
{424, "Failed Dependency"},
{425, "Unassigned"},
{426, "Upgrade Required"},
{428, "Precondition Required"},
{429, "Too Many Requests"},
{431, "Request Header Fields Too Large"},
{500, "Internal Server Error"},
{501, "Not Implemented"},
{502, "Bad Gateway"},
{503, "Service Unavailable"},
{504, "Gateway Timeout"},
{505, "HTTP Version Not Supported"},
{507, "Insufficient Storage"},
{508, "Loop Detected"},
{510, "Not Extended"},
{511, "Network Authentication Required"}
};

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
	StatusCode sp;

	//sp.printAll();
	std::cout << "501: " + sp.getPhrase(501) << std::endl;
	std::cout << "404: " + sp.getPhrase(404) << std::endl;
	std::cout << "200: " + sp.getPhrase(200) << std::endl;
}*/
