#include "Request.hpp"

int	main(void)
{
	std::string	data[] = {
		//"POST http://localhost:80/home.html?a=1&b=2&c=3&d=4#fragment HTTP/1.1\r\nHost: localhost:8080\ncontent-length: 2lines\nformat: text\n\nthis is the body firstline\nthis is the body secondline (with a final lf)\n",
		//"DELETE /home.html HTTP/1.1\r\nHost: localhost:8080\ntruc: value\nmachin: chose\n2: val2\n3: val3\n\n",
		//"GET /home.html HTTP/1.1\r\n\n\n",
		"GET /home.html HTTP/1.1\r\nkey:value\nkey2:  value2  \nkey3:value3\n\n",
		//"DELETE /home.html HTTP/1.1\r\nbadkeyval\nbadkey:\n:badval\nkey:val\n\n",
		/*"GET /home.html HTTP/1.1\r\nHost: localhost:8080\n\n",
		"HEAD /home.html HTTP/1.1\r\nHost: localhost:8080\n\n",
		"grosschiasse qui pue",
		"GET / / / / // / / / // \r\n",
		"PUT \n\n bad truc host:port\r\nbodybad\n",
		"HEAD \r\n"*/
	};

	for (int i = 0; i < 1; ++i)
	{	
		Request	rq(data[i]);
		rq.print();
		std::cout << "------------------------------------------" << std::endl;
	}
}


/*	main request line
int	main()
{
	RequestLine	rql("GET /home.html HTTP/1.1\r\n");

	std::cout << rql.getFull() << std::endl;
	std::cout << rql.getVerb() << std::endl;
	std::cout << rql.getUrl() << std::endl;
	std::cout << rql.getVersion() << std::endl;

}*/


/*	main url
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
