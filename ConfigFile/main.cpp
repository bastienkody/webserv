#include "ConfigFile.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (std::cerr << "arg please" << std::endl, 1);

	ConfigFile	config(av[1]);

	try{
		config.openReadFileToStr();
		std::cout << config.getRawData()<< std::endl;
		config.readAllInfos();
		//std::cout << "\n####\nlocation path:" + config.getServers().front().getLocations().front().getPath() << std::endl;
		//std::cout << "location path absolute?:" << config.getServers().front().getLocations().front().getIsPathAbsolute() << std::endl;
		std::cout << "\n####\nlocation index:" << config.getServers().front().getLocations().front().getIndex() << std::endl;
		std::cout << "location autoindex:" << config.getServers().front().getLocations().front().getAutoIndex() << std::endl;
		std::cout << "location maxbodysize:" << config.getServers().front().getLocations().front().getMaxBodySize() << std::endl;
		std::cout << "location redirection:" << config.getServers().front().getLocations().front().getRedirection() << std::endl;
		std::map<std::string,std::string> errPag = config.getServers().front().getLocations().front().getErrorPages();
		for (std::map<std::string,std::string>::iterator it = errPag.begin(); it != errPag.end(); ++it)
			std::cout << "errPages: key:" + it->first + "|val:" + it->second << std::endl;
		std::vector<std::string> alloMet = config.getServers().front().getLocations().front().getAllowMethods();
		for (std::vector<std::string>::iterator it = alloMet.begin(); it != alloMet.end(); ++it)
			std::cout << "allow methods:" + *it << std::endl;
		std::vector<std::string> cgiExt = config.getServers().front().getLocations().front().getCgiExt();
		for (std::vector<std::string>::iterator it = cgiExt.begin(); it != cgiExt.end(); ++it)
			std::cout << "cgi ext:" + *it << std::endl;
		std::vector<std::string> cgiPath = config.getServers().front().getLocations().front().getCgiPathes();
		for (std::vector<std::string>::iterator it = cgiPath.begin(); it != cgiPath.end(); ++it)
			std::cout << "cgi path:" + *it << std::endl;
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}
