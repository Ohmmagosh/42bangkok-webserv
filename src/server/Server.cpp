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

Server::Server(): server_fd(0), running(false), MAX_CLIENTS(1024), rateLimiter(16000, 1600), currentClientCount(0)
{
    serverPortNamePairs.push_back(std::make_pair(8080, "abc"));
    serverPortNamePairs.push_back(std::make_pair(8081, "anothername"));
    serverPortNamePairs.push_back(std::make_pair(9090, "yetanothername"));

	this->MAX_BODY_SIZE = 10000;
	this->dlpath = ".";
	this->dlname = "lorem7000.txt"; 
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
    Console::modeMsg(1, "Stopping server.");
    closeActiveClients();
    closeServerSocket();
    running = false;
}

void Server::closeActiveClients()
{
    std::set<int>::iterator it;
    for(it = active_clients.begin(); it != active_clients.end(); ++it)
    {
        int client_fd = *it;
        close(client_fd);
    }
    active_clients.clear();
}

void Server::closeServerSocket()
{
    if (server_fd != -1)
    {
        close(server_fd);
        server_fd = -1;
    }
}

std::string Server::handleHttpRequest(const std::string& method, const std::string& path, const std::string& protocol, const std::string& hostHeader)
{
    (void)protocol;
    HttpRequestHandle ret(method, path);

    if (method == "GET" && path == "/download-latest-file")
    {
        return handleFileDownloadRequest();
    }

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
		Response res(400, "Bad Request", "Invalid Host header");
        // return generateHttpResponse(400, "Bad Request", "Invalid Host header");
		return res.HttpResponse();
    }

    return ret.validateMethod();
}

std::string getMimeType(const std::string& filename) 
{
    std::map<std::string, std::string> mimeTypes;

    mimeTypes[".txt"] = "text/plain";
    mimeTypes[".jpg"] = "image/jpeg";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".html"] = "text/html";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".js"] = "application/javascript";

    std::string::size_type idx = filename.rfind('.');
    if (idx != std::string::npos)
	{
        std::string extension = filename.substr(idx);
        if (mimeTypes.find(extension) != mimeTypes.end())
		{
            return mimeTypes[extension];
        }
    }
    return "application/octet-stream";
}

std::string Server::handleFileDownloadRequest() 
{
    std::string fullPath = dlpath + "/" + dlname;

    // Open the file
    std::ifstream file(fullPath, std::ios::binary);
    if (!file.is_open())
    {
        return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    }

    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::string mimeType = getMimeType(dlname);

    // Create the response
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << mimeType << "\r\n";
    response << "Content-Disposition: attachment; filename=\"" << dlname << "\"\r\n";
    response << "Content-Length: " << fileContents.size() << "\r\n";
    response << "\r\n";
    response << fileContents;

    return response.str();
}

void Server::setupSignalHandlers()
{
    signal(SIGINT, Server::signal_handler);
}

int Server::setupKqueue()
{
    int kq = kqueue();
    if (kq < 0)
    {
        perror("Error creating kqueue");
    }
    return kq;
}

void Server::setupServerSockets(int kq, size_t NUM_SERVERS)
{
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
}

void Server::handleEventError(struct kevent& event) 
{
    std::cerr << "Error in kevent: " << strerror(event.data) << std::endl;
}

void Server::handleNewClientConnection(int kq, int& currentClientCount, int eventIdent) 
{
	if (currentClientCount < MAX_CLIENTS)
	{
		struct sockaddr_in client_address;
		socklen_t client_addrlen = sizeof(client_address);
		int new_socket = accept(eventIdent, (struct sockaddr*)&client_address, &client_addrlen);
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
		}
		// Add the new_socket to your data structures and update the count of active clients
		active_clients.insert(new_socket);
		this->currentClientCount++;
	}
	else
	{
		Response res(503, "Service Unavailable", "Server is overloaded");

		struct sockaddr_in client_address;
		socklen_t client_addrlen = sizeof(client_address);
		int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
		if (new_socket >= 0)
		{
			active_clients.insert(new_socket);
			
			send(new_socket, res.HttpResponse().c_str(), res.size(), 0);
			close(new_socket);
		}
	}
}

void Server::handleClientRead(int kq, int eventIdent) 
{
	std::cout << "Attempting recv on FD: " << eventIdent << std::endl;

    struct kevent event;
    int numEvents = kevent(kq, NULL, 0, &event, 1, NULL);
    if (numEvents == -1) {
        perror("kevent error");
        return;
    }

	if (event.flags & EV_EOF)
	{
		active_clients.erase(eventIdent);

		struct kevent client_change;
		EV_SET(&client_change, eventIdent, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
		{
			perror("Error deleting client event from kqueue");
		}

		client_write_queues.erase(eventIdent);
		close(eventIdent);
		this->currentClientCount--;
		return;
	}
	if(fcntl(eventIdent, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
	{
		std::cout << "Invalid file descriptor: " << eventIdent << std::endl;
		return;
	}

	char buffer[READ_BUFFER_SIZE];
	size_t bytesRead = recv(eventIdent, buffer, sizeof(buffer) - 1, 0);
	buffer[bytesRead] = '\0';


	// std::cout << "------- This is next -------" << std::endl;
	std::cout << "\n" << buffer << '\n' << bytesRead << "bytes\n" << "\n" << std::endl;
	// std::cout << "------- This is next -------" << std::endl;

	if (bytesRead < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// Non-blocking operation would block, continue processing other events
			return;
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
		active_clients.erase(eventIdent);


		struct kevent client_change;
		EV_SET(&client_change, eventIdent, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
		{
			perror("Error deleting client event from kqueue");
		}

		client_write_queues.erase(eventIdent);
		close(eventIdent);
		currentClientCount--;
	}
	else
	{

		buffer[bytesRead] = '\0';
		std::string requestData(buffer);
		Request parsedRequest;
		std::string hostHeader;
		try 
		{
			parsedRequest = Request(requestData);
			hostHeader = parsedRequest.getHeaderValue("Host");
		} 
		catch (const Request::HeaderNotFound& e) 
		{
			std::cerr << "Error: " << e.what() << std::endl;
			Response res(400, "Bad Request", "Host header not found");
			send(eventIdent, res.HttpResponse().c_str(), res.size(), MSG_NOSIGNAL);
		}

		if (parsedRequest.getBody().size() > MAX_BODY_SIZE)
		{
			Response res(413, "Payload Too Large", "Request body is too large");
			send(eventIdent, res.HttpResponse().c_str(), res.size(), 0);
			
			return;
		}

		if (rateLimiter.consume())
		{
			std::string httpResponse = handleHttpRequest(parsedRequest.getMethod(), parsedRequest.getPath(), parsedRequest.getProtocol(), hostHeader);

			// Push the response into the client's write queue
			client_write_queues[eventIdent].push(httpResponse);

			// Add an EVFILT_WRITE event to the kqueue to indicate that the socket is ready for writing
			struct kevent client_change;
			EV_SET(&client_change, eventIdent, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
			kevent(kq, &client_change, 1, NULL, 0, NULL);
		}
		else
		{
			Response res(429, "Too Many Requests", "Rate limit exceeded");
			send(eventIdent, res.HttpResponse().c_str(), res.size(), MSG_NOSIGNAL);
		}

		if (got_signal)
		{
			stop();
			return;
		}
	}
}

void Server::handleClientWrite(int kq, int eventIdent)
{
	struct kevent event;
    int numEvents = kevent(kq, NULL, 0, &event, 1, NULL);
    if (numEvents == -1) {
        perror("kevent error");
        return;
    }
	std::cout << "Handling write event for fd: " << eventIdent << std::endl;

	std::map<int, std::queue<std::string> >::iterator it = client_write_queues.find(eventIdent);
	if (it != client_write_queues.end())
	{
		std::queue<std::string>& write_queue = it->second;

		while (!write_queue.empty())
		{
			const std::string& data_to_send = write_queue.front();
			ssize_t bytes_sent = send(eventIdent, data_to_send.c_str(), data_to_send.size(), 0);

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
					active_clients.erase(eventIdent);
					client_write_queues.erase(eventIdent);
					close(eventIdent);
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
					EV_SET(&kev, eventIdent, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0)
					{
						perror("Error removing write event from kqueue");
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

void Server::mainEventLoop(int kq, int& currentClientCount, size_t NUM_SERVERS)
{
	while (running)
	{

		struct kevent events[NUM_SERVERS + MAX_CLIENTS];
		int numEvents = kevent(kq, NULL, 0, events, NUM_SERVERS + MAX_CLIENTS, NULL);
		if (numEvents < 0)
		{
			perror("Error in kevent");
			return ;
		}

		for (int i = 0; i < numEvents; ++i)
		{
			struct kevent event = events[i];

			if (event.flags & EV_ERROR)
			{
				handleEventError(event);
				continue;
			}
			if (std::find(serverSockets.begin(), serverSockets.end(), event.ident) != serverSockets.end())
			{
				handleNewClientConnection(kq, currentClientCount, event.ident);
			}
			else if (event.filter == EVFILT_READ)
			{
				handleClientRead(kq, event.ident);
			}
			else if (event.filter == EVFILT_WRITE)
			{
				handleClientWrite(kq, event.ident);
			}
		}
	}
}

void Server::start()
{
	setupSignalHandlers();

	int kq = setupKqueue();

	size_t NUM_SERVERS = serverPortNamePairs.size();
	setupServerSockets(kq, NUM_SERVERS);

	running = true;

	mainEventLoop(kq, currentClientCount, NUM_SERVERS);
}
