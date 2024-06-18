#include "ConfigFile.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (std::cerr << "arg please" << std::endl, 1);

	ConfigFile	config(av[1]);

	try{
		config.openReadFileToStr();
		config.readAllInfos();
		config.printAll();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}
