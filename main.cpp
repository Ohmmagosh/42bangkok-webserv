#include "src/server/Server.hpp"

Server server;

void signalHandler(int signum) 
{
    std::cout << "Received signal " << signum << ". Shutting down server..." << std::endl;

    server.shutdown();

    exit(signum);
}

// int main()
// {
//     signal(SIGINT, signalHandler);

//     // Server serv;
//     server = Server();

//     while (!std::cin.eof())
//     {
//         int result = server.startsocket();
//         if (result != 0)
//             break;

//         sleep(1);
//     }
//     server.shutdown();
// }

int main()
{
    int const x = 5;
    const int y = 5;

    x = 10;
    // y = y + 7;

}