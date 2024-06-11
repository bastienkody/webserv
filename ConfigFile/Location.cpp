#include"Location.hpp"


void	Location::readInfos(std::string & raw)
{
	std::cout << "from location.readinfo() :" << std::endl;
	std::string	line, rawLine;

	while (raw.find('\n') != std::string::npos)
	{
		rawLine = raw.substr(0, raw.find('\n'));
		line = ParserUtils::trimOWS(ParserUtils::eraseSufixComments(rawLine));
		raw.erase(0, raw.find('\n') + 1);
		if (line.size() == 0) // skip empty line
			continue;
		std::cout << line;
		if (line.compare("}") == 0) // get back to config part
			break;
		else
			std::cout << "location attribute" << std::endl;
	}
}
