#include "Request.hpp"

int	main(void)
{
	std::string	data[] = {
		"GET /home.html HTTP/1.1\r\nHost: localhost:8080\ntruc: value\n\nthis is the body\n"
	};

	Request	rq(data[0]);
	rq.print();
}




/*
MAIN URl SOLO
int	main(void)
{
	std::string urls[] = {
		"/",
		"/path/somefile.html",
		"/path?query=value",
		"/path?key=val&key2=val2",
		"/path?query=value#anchor",
		"http://www.example.com:80/path/to/myfile.html?key1=value1&key2=value245#SomewhereInTheDocument"
		};

	for (int i = 0; i<6; ++i)
	{
		URL	url(urls[i]);
		url.printDebug();
		std::cout << "#########################################################" << std::endl;
	}
}
*/