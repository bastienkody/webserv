#include "ConfigFile.hpp"

int	main(void)
{
	ConfigFile	config("configfile");

	try{
		config.openReadFileData();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}
