#include "ConfigFile.hpp"

int	main(void)
{
	ConfigFile	config("testlucas.config");

	try{
		config.openReadFileData();
		std::cout << config.getRawData()<< std::endl;
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}
