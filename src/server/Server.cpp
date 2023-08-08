/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:29 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/08 14:19:27 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() 
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        throw (SocketCreationException());
    }

    std::ifstream file("./src/server/index.html"); // จริงๆ ไฟล์แรกอ่านตาม config
    if (file.is_open())
    {
        std::cout << "hello" <<std::endl;
        std::string content;
        char ch;
        while (file.get(ch)) 
        {
            content.push_back(ch);
        }
        this->httpResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
        this->httpResponse.append(content);
    }
    else
    {
        this->httpResponse = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n"
                            "<html>"
                            "<head><title>404 Not Found</title></head>"
                            "<body>"
                            "<h1>404 Not Found</h1>"
                            "<p>The requested resource was not found on this server.</p>"
                            "</body>"
                            "</html>";
    }

    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(8080); // อันนี้ต้องปรับ port ตาม config file ไว้มีแล้วค่อยแก้
    this->server_addr.sin_addr.s_addr = INADDR_ANY;
}



Server::Server(Server const & src)
{
    *this = src;
}

int Server::startsocket()
{
    if (bind(this->sockfd, (struct sockaddr*)&this->server_addr, sizeof(this->server_addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    listen(this->sockfd, 10);

    std::vector<pollfd> fds;
    pollfd newFd;
    newFd.fd = this->sockfd;
    newFd.events = POLLIN;
    newFd.revents = 0;
    fds.push_back(newFd);    

    
    for (size_t i = 0; i < fds.size(); i++)
    {
        if (fds[i].revents & POLLIN)
        {
            if (fds[i].fd == this->sockfd)
            {
                sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_sockfd = accept(this->sockfd, (struct sockaddr*)&client_addr, &client_len);
                if (client_sockfd < 0)
                {
                    std::cerr << "Error accepting connection" << std::endl;
                    return 1;
                }

                pollfd clientFd;
                clientFd.fd = client_sockfd;
                clientFd.events = POLLIN;
                clientFd.revents = 0;
                fds.push_back(clientFd);
            }
            else
            {
                // Handle existing client
                char buffer[4096];
                int bytes_read = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                // ... (handle client)
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                i--; // Adjust loop counter because of the removed element
            }
        }
    }
}    

Server::~Server()
{
    close(this->sockfd);
}

void Server::shutdown()
{
    close(this->sockfd);

    std::cout << "Server shutdown completed." << std::endl;
}

Server &	Server::operator=(Server const & rhs)
{
    if (this != &rhs)
    {
        //do something
    }
    return (*this);
}

