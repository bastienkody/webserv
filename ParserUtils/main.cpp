#include "ParserUtils.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (std::cerr << "Arg please" << std::endl, 1);

	//std::cout << ParserUtils::eraseOWS(av[1]) << std::endl;
	//std::cout << ParserUtils::firstWsPos(av[1]) << std::endl;
	//std::string line(av[1]); std::cout << ParserUtils::checkRmSemiColon(line) << "\tline: " + line <<std::endl;
}
