#include "src/server/Server.hpp"

// Server server;

// void signalHandler(int signum) 
// {
//     std::cout << "Received signal " << signum << ". Shutting down server..." << std::endl;

//     server.shutdown();

//     exit(signum);
// }

// int main()
// {
//     // signal(SIGINT, signalHandler);

//     // // Server serv;
//     // server = Server();

//     // while (!std::cin.eof())
//     // {
//     //     int result = server.startsocket();
//     //     if (result != 0)
//     //         break;

//     //     sleep(1);
//     // }
//     // server.shutdown();
//     signal(SIGINT, signalHandler);

//     try 
//     {
//         Server server;

//         server.startsocket();

//         server.shutdown();
//     } 
//     catch (const std::exception& e) 
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }

//     return 0;
// }

// int main()
// {
//     int const x = 5;
//     const int y = 5;

//     x = 10;
//     // y = y + 7;

// }

int main()
{
    Server server(8080);
    while (true)
    {
        // The main loop where you'll accept and handle connections (covered in subsequent steps)
    }
    return 0;
}