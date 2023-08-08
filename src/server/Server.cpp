#include "Server.hpp"

Server::Server(int port)
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        std::cerr << "Error creating socket!" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::memset(&server_addr, 0, sizeof(server_addr)); // เคลีย struct

    //ini struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Binding failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 10) < 0)
    {
        std::cerr << "Error on listen!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << port << std::endl;
    
    this->setupSocket();
}

void Server::run()
{
    while (true) {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0)
        {
            perror("poll");
            break;
        }

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].fd == sockfd && fds[i].revents & POLLIN)
            {
                // handleNewConnection();
            }
            else if (fds[i].revents & POLLIN)
            {
                // handleClientData(i);
            }
        }
    }
}

void Server::setupSocket()
{
    pollfd serverFd;
    serverFd.fd = sockfd;
    serverFd.events = POLLIN;
    fds.push_back(serverFd);
}

void Server::handleNewConnection() 
{
    // Code to accept new client connection and add to fds vector goes here...
}

// void Server::handleClientData(size_t index) 
// {
//     // Code to handle data from a connected client goes here...
// }

Server::~Server()
{
        if (sockfd != -1)
        {
            close(sockfd);
        }
}

int Server::getSocket() const 
{
    return sockfd;
}
