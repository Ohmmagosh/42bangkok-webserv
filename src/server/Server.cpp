#include "Server.hpp"

Server::Server(int port): port(port), server_fd(0), running(false)
{

}

Server::~Server()
{
	if (running)
	{
		stop();
	}
}

void Server::setNonBlocking()
{
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1) 
    {
        perror("fcntl");
        // stop();
        exit(EXIT_FAILURE);
    }
}

// bool Server::parseHttpRequest(const std::string &request, std::string &method, std::string &path, std::string &protocol)
// {
//     std::istringstream requestStream(request);
//     requestStream >> method >> path >> protocol;

//     // Basic validation to ensure that we've successfully parsed the request
//     return (!method.empty() && !path.empty() && !protocol.empty());
// }

void Server::start()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		std::cerr << "Failed to create socket." << std::endl;
		return;
	}

	// setNonBlocking();

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        std::cerr << "Failed to set SO_REUSEADDR." << std::endl;
        // std::cout << "Failed to set SO_REUSEADDR." << std::endl;
        // stop();
        return;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "Failed to set SO_REUSEPORT." << std::endl;
        // std::cout << "Failed to set SO_REUSEADDR." << std::endl;
        // stop();
        return;
    }


    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
        std::cerr << "Bind failed." << std::endl;
        // stop();
        return;
    }

    if (listen(server_fd, 3) < 0) 
    {
        std::cerr << "Listen failed." << std::endl;
        // stop();
        return;
    }

    int currentClientCount = 0;
    const int MAX_CLIENTS = 1024;
    // std::vector<pollfd> clients;
    // pollfd serverPollFd;
    // serverPollFd.fd = server_fd;
    // serverPollFd.events = POLLIN;
    // serverPollFd.revents = 0;
    // clients.push_back(serverPollFd);
    int kq = kqueue();
    if (kq == -1) {
        std::cerr << "Could not create kqueue" << std::endl;
        return;
    }

    struct kevent changeList[1024];
    struct kevent eventList[1024];
    EV_SET(&changeList[0], server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

	running = true;

    while(running) 
    {
        std::cout << "work!!" << std::endl;
        // int poll_count = poll(&clients[0], clients.size(), -1);
        // std::cout << "poll "<< poll_count << std::endl;
        // if (poll_count < 0) 
        // {
        //     std::cerr << "poll() failed." << std::endl;
        //     std::cout << "poll() failed." << std::endl;
        //     return;
        // }

        // for (size_t i = 0; i < clients.size(); ++i)
        // {
        //     if (clients[i].revents & POLLIN) 
        //     {
        //         if (clients[i].fd == server_fd) 
        //         {
        //             // Handle new client connection
        //             struct sockaddr_in client_address;
        //             socklen_t client_addrlen = sizeof(client_address);
        //             int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
        //             if (new_socket >= 0 && clients.size() < MAX_CLIENTS) 
        //             {
        //                 // std::cout << "Client connected!" << std::endl;
        //                 pollfd newClientPollFd;
        //                 newClientPollFd.fd = new_socket;
        //                 newClientPollFd.events = POLLIN;
        //                 newClientPollFd.revents = 0;
        //                 clients.push_back(newClientPollFd);
        //             }
        //         }
        int newEvents = kevent(kq, changeList, 1, eventList, 1, NULL);
        if (newEvents < 0)
        {
            std::cerr << "kevent failed" << std::endl;
            return;
        }


        for (int i = 0; i < newEvents; i++)
        {
            struct kevent event = eventList[i];

            if (event.flags & EV_ERROR)
            {
                std::cerr << "Error in kevent: " << strerror(event.data) << std::endl;
                continue;
            }
            if (event.ident == static_cast<uintptr_t>(server_fd))
            // if (event.ident == server_fd)
            {
                if (currentClientCount < MAX_CLIENTS)
                {
                    struct sockaddr_in client_address;
                    socklen_t client_addrlen = sizeof(client_address);
                    int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
                    if (new_socket < 0)
                    {
                        std::cerr << "Failed to accept new connection" << std::endl;
                        continue; // Skip the rest of the loop
                    }

                    setNonBlocking();
                    // // Set non-blocking for client socket
                    // if (fcntl(new_socket, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1) 
                    // {
                    //     perror("fcntl");
                    //     close(new_socket); // Close the client socket as it's unusable now
                    //     continue; // Skip the rest of the loop
                    // }

                    if (new_socket >= 0)
                    {
                        struct kevent client_change;
                        EV_SET(&client_change, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                        if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
                        {
                            std::cerr << "Could not add client event to kqueue" << std::endl;
                        }
                        currentClientCount++;
                    }
                }
                else
                {
                    std::cerr << "Max clients reached. Cannot accept more." << std::endl;
                    std::string httpResponse = generateHttpResponse(503, "Service Unavailable", "Server is overloaded");

                    struct sockaddr_in client_address;
                    socklen_t client_addrlen = sizeof(client_address);
                    int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
                    if (new_socket >= 0)
                    {
                        send(new_socket, httpResponse.c_str(), httpResponse.size(), 0);
                        close(new_socket);
                    }
                }
            }
            else 
            {
                // Handle data from a client or client disconnection
                char buffer[4096];  // 4K buffer for receiving data
                ssize_t bytesRead = recv(event.ident, buffer, sizeof(buffer) - 1, 0);

                // std::cout << "---------\n" << buffer << '\n' << "---------\n" << std::endl;

                if (bytesRead <= 0) 
                {
                    std::cerr << "b < 0" << std::endl;
                    // Client disconnected or error occurred

                    struct kevent client_change;
                    EV_SET(&client_change, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
                    {
                        std::cerr << "Error: " << strerror(errno) << std::endl;
                        std::cerr << "Could not delete client event from kqueue" << std::endl;
                    }
                    close(event.ident);
                    currentClientCount--;
                } 
                else 
                {
                    buffer[bytesRead] = '\0';  // Null-terminate the received data
                    std::string request(buffer);

                    // Extract HTTP request details
                    std::istringstream requestStream(request);
                    std::string method, path, protocol;
                    requestStream >> method >> path >> protocol;

                    // std::cout << request << std::endl;
                    // std::cout << "+++" << method << std::endl;
                    // Use the extracted details to handle the HTTP request
                    std::string httpResponse = handleHttpRequest(method, path, protocol);

                    // Send the HTTP response back to the client
                    send(event.ident, httpResponse.c_str(), httpResponse.size(), 0);
                }
            }
        }
    }
}

void Server::stop()
{
    int result;
    for (std::vector<pollfd>::iterator it = clients.begin(); it != clients.end(); ++it) 
    {
        result = close(it->fd);
        if (result < 0)
        {
            perror("Failed to close client socket");
        }
    }
    result = close(server_fd);
    if (result < 0)
    {
        perror("Failed to close server socket");
    }
    running = false;
}

std::string Server::handleHttpRequest(const std::string& method, const std::string& path, const std::string& protocol)
{
	(void)protocol;
	if (method == "GET")
	{
		if (path == "/" || path == "/index.html")
		{
			std::ifstream file("./src/server/index.html", std::ios::in | std::ios::binary);
			if (file.is_open())
			{
				std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
				return generateHttpResponse(200, "OK", content);
			}
			else
			{
				return generateHttpResponse(404, "Not Found", "File not found");
			}
		}
		else
		{
			return generateHttpResponse(404, "Not Found", "File not found");
		}
	}
	// ... Handle other HTTP methods here

	return generateHttpResponse(501, "Not Implemented", "This method is not implemented");
}

std::string Server::generateHttpResponse(int statusCode, const std::string& statusMessage, const std::string& content)
{
	std::ostringstream response;
	response << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
	response << "Content-Length: " << content.size() << "\r\n";
	response << "\r\n";  // End of headers
	response << content;
	return response.str();
}
