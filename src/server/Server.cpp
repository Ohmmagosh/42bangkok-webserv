#include "Server.hpp"
#include "TBucket.hpp"
#include "Request.hpp"

volatile sig_atomic_t Server::got_signal = 0;

void Server::signal_handler(int sig)
{
	(void)sig;
	got_signal = 1;
}

std::string Server::extractHostHeader(const std::string& request)
{
    std::string::size_type startPos = request.find("Host: ");
    if (startPos == std::string::npos)
	{
        return ""; // Host header not found
    }
    startPos += 6; // Move past "Host: "
    std::string::size_type endPos = request.find("\r\n", startPos);
    if (endPos == std::string::npos)
	{
        return ""; // Malformed request
    }
    return request.substr(startPos, endPos - startPos);
}

Server::Server(): server_fd(0), running(false), MAX_CLIENTS(1024), rateLimiter(16000, 1600)
{
    serverPortNamePairs.push_back(std::make_pair(8080, "abc"));
    serverPortNamePairs.push_back(std::make_pair(8081, "anothername"));
    serverPortNamePairs.push_back(std::make_pair(9090, "yetanothername"));
}

Server::~Server()
{
	if (running)
	{
		stop();
	}
}

void Server::stop()
{
	std::cout << "Stopping server." << std::endl;

	std::set<int>::iterator it;
	for(it = active_clients.begin(); it != active_clients.end(); ++it)
	{
		int client_fd = *it;
		close(client_fd);
	}
	active_clients.clear();

	if (server_fd != -1)
	{
		close(server_fd);
		server_fd = -1;
	}
	running = false;
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

std::string Server::handleHttpRequest(const std::string& method, const std::string& path, const std::string& protocol, const std::string& hostHeader)
{
    (void)protocol;
    HttpRequestHandle ret(method, path);
    // std::cout << "method : " << method << " path: " << path << std::endl;

    // Validate the Host header
	bool validHost = false;
	for (std::vector<std::pair<int, std::string> >::const_iterator it = serverPortNamePairs.begin(); it != serverPortNamePairs.end(); ++it) {
		std::stringstream ss;
		ss << it->second << ":" << it->first;
		std::stringstream localhostWithPort;
		localhostWithPort << "localhost:" << it->first;
		std::stringstream ipWithPort;
		ipWithPort << "127.0.0.1:" << it->first;
		if (hostHeader == ss.str() || hostHeader == localhostWithPort.str() || hostHeader == ipWithPort.str() || hostHeader == "10.13.8.3:" + std::to_string(it->first)) {
			validHost = true;
			break;
		}
	}

    if (!validHost) {
        return generateHttpResponse(400, "Bad Request", "Invalid Host header");
    }

    return ret.validateMethod();
	// if (method == "GET")
	// {
	// 	if (path == "/" || path == "/index.html")
	// 	{
	// 		std::ifstream file("./src/server/index.html", std::ios::in | std::ios::binary);
	// 		if (file.is_open())
	// 		{
	// 			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	// 			file.close();
	// 			return generateHttpResponse(200, "OK", content);
	// 		}
	// 		else
	// 		{
	// 			return generateHttpResponse(404, "Not Found", "File not found");
	// 		}
	// 	}
	// 	else
	// 	{
	// 		return generateHttpResponse(404, "Not Found", "File not found");
	// 	}
	// }
	// ... Handle other HTTP methods here

	// return generateHttpResponse(501, "Not Implemented", "This method is not implemented");

}

void Server::start()
{
	// signal(SIGINT, Server::signal_handler);
	// signal(SIGTERM, Server::signal_handler);
	// signal(SIGPIPE, SIG_IGN);

	int kq = kqueue();
	if (kq < 0)
	{
		perror("Error creating kqueue");
		return ;
	}

	size_t NUM_SERVERS = serverPortNamePairs.size();

	for (size_t i = 0; i < NUM_SERVERS; ++i)
	{
		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
		{
			perror("Error creating socket");
			return ;
		}


		int opt = 1;
		if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			perror("setsockopt SO_REUSEADDR");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
        int port = serverPortNamePairs[i].first;
        // std::string name = serverPortNamePairs[i].second;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

		if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
			perror("Error binding socket");
			return ;
		}

		std::cout << "Server socket created with FD: " << serverSocket << std::endl;

		if (listen(serverSocket, 1024) < 0) {
			perror("Error listening");
			return ;
		}

		std::cout << "Server " << i + 1 << " listening on port " << serverPortNamePairs[i].first << std::endl;

		fcntl(serverSocket, F_SETFL, O_NONBLOCK | FD_CLOEXEC);

		serverSockets.push_back(serverSocket);

		struct kevent kev;
		EV_SET(&kev, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0)
		{
			perror("Error registering server socket with kqueue");
			return ;
		}
	}

	int currentClientCount = 0;
	running = true;

	while (running)
	{
		// std::cout << "running" << std::endl;

		struct kevent events[NUM_SERVERS + MAX_CLIENTS];
		// struct kevent changeList[1024];
		// int numEvents = kevent(kq, changeList, 1, events, 1024, NULL);
		int numEvents = kevent(kq, NULL, 0, events, NUM_SERVERS + MAX_CLIENTS, NULL);
		if (numEvents < 0)
		{
			perror("Error in kevent");
			return ;
		}

		for (int i = 0; i < numEvents; ++i)
		{
			struct kevent event = events[i];

			// std::cout << "---------\n" << i << '\n' << "---------\n" << std::endl;

			if (event.flags & EV_ERROR)
			{
				std::cerr << "Error in kevent: " << strerror(event.data) << std::endl;
				continue;
			}
			// if (event.ident == static_cast<uintptr_t>(server_fd))
			if (std::find(serverSockets.begin(), serverSockets.end(), event.ident) != serverSockets.end())
			{
				// std::cout << "---------\n" << "nee!" << "---------\n" << std::endl;
				// exit(0);
				if (currentClientCount < MAX_CLIENTS)
				{
					struct sockaddr_in client_address;
					socklen_t client_addrlen = sizeof(client_address);
					int new_socket = accept(event.ident, (struct sockaddr*)&client_address, &client_addrlen);
					if (new_socket < 0)
					{
						perror("new_socket");
					}

					std::cout << "New client connected with FD: " << new_socket << std::endl;

					if (fcntl(new_socket, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
					{
						perror("fcntl nonblock");
						close(new_socket);
					}

					struct kevent kev;
					EV_SET(&kev, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
					if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0)
					{
						perror("Error registering new client socket with kqueue");
						// Handle error
					}
					// Add the new_socket to your data structures and update the count of active clients
					active_clients.insert(new_socket);
					currentClientCount++;
				}
				else
				{
					std::string httpResponse = generateHttpResponse(503, "Service Unavailable", "Server is overloaded");

					struct sockaddr_in client_address;
					socklen_t client_addrlen = sizeof(client_address);
					int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
					if (new_socket >= 0)
					{
						active_clients.insert(new_socket);
						
						// std::cout << httpResponse.c_str() << std::endl;
						send(new_socket, httpResponse.c_str(), httpResponse.size(), 0);

						close(new_socket);
					}
				}
			}
			else if (event.filter == EVFILT_READ)
			{
				std::cout << "Attempting recv on FD: " << event.ident << std::endl;
				if (event.flags & EV_EOF)
				{
					active_clients.erase(event.ident);

					struct kevent client_change;
					EV_SET(&client_change, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
					{
						perror("Error deleting client event from kqueue");
					}

					client_write_queues.erase(event.ident);
					close(event.ident);
					currentClientCount--;
					continue;
				}
				if(fcntl(event.ident, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
				{
					std::cout << "Invalid file descriptor: " << event.ident << std::endl;
					continue;
				}
				char buffer[4096];
				ssize_t bytesRead = recv(event.ident, buffer, sizeof(buffer) - 1, 0);

				// std::cout << "---------\n" << buffer << '\n' << bytesRead << "bytes\n" << "---------\n" << std::endl;

				if (bytesRead < 0)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
					{
						// Non-blocking operation would block, continue processing other events
						continue;
					}
					else
					{
						perror("recv error");
						// Handle other errors or disconnect here
						// You may want to close the socket or take other appropriate actions
						// For example, you can remove the client from your data structures
					}
				}
				else if (bytesRead == 0)
				{
					active_clients.erase(event.ident);


					struct kevent client_change;
					EV_SET(&client_change, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
					{
						perror("Error deleting client event from kqueue");
					}

					client_write_queues.erase(event.ident);
					close(event.ident);
					currentClientCount--;
				}
				else
				{
					buffer[bytesRead] = '\0';
					std::string request(buffer);
					std::string hostHeader = extractHostHeader(request);

					// Print the request to the console
					std::cout << "Received request:\n" << request << "\n-------------------\n";

					if (rateLimiter.consume())
					{
						Request requestString(request);
						std::string httpResponse = handleHttpRequest(requestString.getMethod(), requestString.getPath(), requestString.getProtocol(), hostHeader);

						// Push the response into the client's write queue
						client_write_queues[event.ident].push(httpResponse);

						// Add an EVFILT_WRITE event to the kqueue to indicate that the socket is ready for writing
						struct kevent client_change;
						EV_SET(&client_change, event.ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
						kevent(kq, &client_change, 1, NULL, 0, NULL);
					}
					else
					{
						std::string httpResponse = generateHttpResponse(429, "Too Many Requests", "Rate limit exceeded");
						// std::cout << httpResponse.c_str() << std::endl;
						send(event.ident, httpResponse.c_str(), httpResponse.size(), MSG_NOSIGNAL);
					}

					if (got_signal)
					{
						stop();
						break;
					}
				}
			}
			else if (event.filter == EVFILT_WRITE)
			{
				std::cout << "Handling write event for fd: " << event.ident << std::endl;

				std::map<int, std::queue<std::string> >::iterator it = client_write_queues.find(event.ident);
				if (it != client_write_queues.end())
				{
					std::queue<std::string>& write_queue = it->second;

					// While there's something to send to this client
					while (!write_queue.empty())
					{
						const std::string& data_to_send = write_queue.front();
						std::cout << ",.,.,.,.,.,.,.,.,.,.,.,.,.,.\n" << data_to_send.c_str() << ",.,.,.,.,.,.,.,.,.,.,.,.,.,.\n" << std::endl;
						ssize_t bytes_sent = send(event.ident, data_to_send.c_str(), data_to_send.size(), 0);

						if (bytes_sent < 0)
						{
							if (errno == EAGAIN || errno == EWOULDBLOCK)
							{
								// Would block, so stop trying to send more data
								break;
							}
							else
							{
								perror("send error");
								// Remove client from active_clients and close its socket
								active_clients.erase(event.ident);
								client_write_queues.erase(event.ident);
								close(event.ident);
								currentClientCount--;
								break;
							}
						}
						else
						{
							write_queue.pop();

							// If we have sent all the data, then let's remove the write event from kqueue
							if (write_queue.empty())
							{
								struct kevent kev;
								EV_SET(&kev, event.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
								if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0)
								{
									perror("Error removing write event from kqueue");
									// Handle error
								}
							}
						}
					}
				}
				else
				{
					std::cerr << "No write queue found for client with FD: " << event.ident << std::endl;
				}
			}
		}
	}
}
