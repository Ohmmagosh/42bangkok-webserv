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
    while (true) 
    {
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
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (client_sockfd < 0)
    {
        perror("accept");
        return;
    }

    if (fcntl(client_sockfd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
    {
        perror("fcntl");
        close(client_sockfd);
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        close(client_sockfd);
        return;
    }

    if (pid == 0) 
    {
        close(sockfd);
        
        // Call a function to handle client data, communication, etc.
        handleClient(client_sockfd);
        
        exit(EXIT_SUCCESS);
    } 
    else
    {  // This block will be executed by the parent process
        close(client_sockfd);  // Parent doesn't need the client socket
    }
}

// void Server::handleClientData(size_t index) 
// {
//     // Code to handle data from a connected client goes here...
// }

void Server::handleClient(int client_sockfd)
{
    char buffer[4096];
    
    // Here, you can implement a loop to continuously read data from the client
    // until they disconnect or you can simply read once depending on your design.

    int bytes_read = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
    {
        perror("recv");
        close(client_sockfd);
        return;
    }

    // parser แก้พาร์ทนี้ไปรีเชคเออเรอ กับรายละเอียดให้ด้วย
    std::istringstream requestStream(buffer);
    std::string requestMethod, requestPath, httpVersion;
    requestStream >> requestMethod >> requestPath >> httpVersion;
    // ถึงตรงนี้

    buffer[bytes_read] = '\0';  // Null-terminate the received data
    std::cout << "Received: " << buffer << std::endl;

    // Here, you can handle the client's request and send back a response
    // For now, let's just echo back the received data
    send(client_sockfd, buffer, bytes_read, 0);

    close(client_sockfd);
}

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
