#include "ConfigFile.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (std::cerr << "arg please" << std::endl, 1);

	ConfigFile	config(av[1]);

	try{
		config.openReadFileToStr();
		config.readAllInfos();
		for (std::vector<Location>::const_iterator it = config.getServers().front().getLocations().begin(); it != config.getServers().front().getLocations().end(); ++it)
			it->printAll();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}
