#include "src/server/Server.hpp"
#include "src/stdlib.hpp"
#include "src/parsing/Conf.hpp"
#include "src/struct.hpp"

t_globalConf g_conf;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file_name>" << std::endl;
		return 1;
	}
	Conf confParser;
	if (!confParser.parseConfigFile(argv[1]))
	{
		std::cerr << "Error parsing the configuration file." << std::endl;
		return 1;
	}

	g_conf.default_error_pages = confParser.getGlobalDefualtErrorPage();
	g_conf.client_body_limit = confParser.getClientBodyLimit();

	Server serv(confParser);
	serv.start();


	return 0;
}
