#include "src/server/Server.hpp"
#include "src/stdlib.hpp"
#include "src/server/configparser.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <config_file_name>" << std::endl;
        return 1;
    }

    std::string configFileName = argv[1];
    ConfigParser parser;
    if (parser.parse(configFileName)) 
    {
        // Print global configurations
        std::cout << "Global Configuration:" << std::endl;
        for (std::map<std::string, std::string>::const_iterator it = parser.globalConfig.begin();
             it != parser.globalConfig.end(); ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }

        // Print server configurations
        std::cout << "\nServers Configuration:" << std::endl;
        for (size_t i = 0; i < parser.servers.size(); ++i) {
            std::cout << "Server " << (i + 1) << ":" << std::endl;
            const std::map<std::string, std::string>& server = parser.servers[i];
            for (std::map<std::string, std::string>::const_iterator it = server.begin();
                 it != server.end(); ++it) {
                std::cout << "  " << it->first << ": " << it->second << std::endl;
            }
            std::cout << std::endl;
        }
    }
    
    std::string conf(argv[1]);
	Server server(conf);
	server.start();
	return 0;
}
