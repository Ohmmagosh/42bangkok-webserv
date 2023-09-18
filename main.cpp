#include "src/server/Server.hpp"
#include "src/stdlib.hpp"

// void startServers(const std::vector<int>& ports) 
// {
//     for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); ++it) {
//         int port = *it;
//         Server server(port);
//         server.start();
//     }
// }

int main()
{
    // std::vector<int> portList;
    // portList.push_back(8080);
    // portList.push_back(8081);
    // portList.push_back(8082);



	// startServers(portList);
	Server server;
	server.start();
	return 0;
}
