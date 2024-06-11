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
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}
