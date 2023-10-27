#include "Server.hpp"
#include "Response.hpp"

volatile sig_atomic_t Server::got_signal = 0;

std::string Server::createSimpleHttpResponse()
{
	std::string body = "<!DOCTYPE html>"
					   "<html>"
					   "<head>"
					   "<title>Test Page</title>"
					   "</head>"
					   "<body>"
					   "<h1>Welcome to My Test Page!</h1>"
					   "<p>This is a simple test page served by your C++ server.</p>"
					   "</body>"
					   "</html>";

	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n";
	response << "Content-Type: text/html; charset=UTF-8\r\n";
	response << "Content-Length: " << body.length() << "\r\n";
	response << "\r\n";  // End of headers
	response << body;

	return response.str();
}

Server::Server()
{
	// config = initializeMockConfig();
}

Server::Server(const Conf& config)
{
	this->_config = config;
}

Server::~Server()
{
	if (this->_running)
	{
		stop();
	}
}

void Server::stop()
{
	Console::log(1, "Stopping server.");
	closeActiveClients();
	closeServerSocket();
	this->_running = false;
}

void Server::signal_handler(int sig)
{
	(void)sig;
	got_signal = 1;
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
	if (this->_server_fd != -1)
	{
		close(this->_server_fd);
		this->_server_fd = -1;
	}
}

void Server::setupSignalHandlers()
{
	signal(SIGINT, Server::signal_handler);
}

void Server::handleClientDisconnection(int kq, int eventIdent)
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
	this->_currentClientCount--;
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
	t_con config;

	config = this->_config.getAllConfig();
	// config.global.
	std::cout << "setupserversocket" << std::endl;
	for (size_t i = 0; i < NUM_SERVERS; ++i)
	{
		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
		{
			perror("Error creating socket");
			std::cerr << "Failed to create server socket for server number " << i << std::endl;
			return;
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
		int port = config.server[i].port;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(port);

		if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
			perror("Error binding socket");
			return ;
		}

		std::cout << "Server socket created with FD: " << serverSocket << std::endl;

		if (listen(serverSocket, 2048) < 0) {
			perror("Error listening");
			return ;
		}

		std::cout << "Server " << i + 1 << " listening on port " << config.server[i].port << std::endl;

		fcntl(serverSocket, F_SETFL, O_NONBLOCK | FD_CLOEXEC);

		this->_serverSockets.push_back(serverSocket);

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

void Server::handleNewClientConnection(int kq, int eventIdent)
{
    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof(client_address);
    int new_socket = accept(eventIdent, (struct sockaddr*)&client_address, &client_addrlen);
    if (new_socket < 0)
    {
		perror("new_socket");
		std::cerr << "Failed to accept client connection on server FD: " << eventIdent << std::endl;
		return;
    }

	struct sockaddr_in server_address;
	socklen_t server_addrlen = sizeof(server_address);
	int server_port = 0;
	if (getsockname(new_socket, (struct sockaddr*)&server_address, &server_addrlen) < 0)
	{
		perror("getsockname");
	}
	else
	{
		server_port = ntohs(server_address.sin_port);
		std::cout << "Connection accepted on server port: " << server_port << std::endl;
	}
	// std::map<int, int> client_to_port_map;
	client_to_port_map[new_socket] = server_port;

	std::cout << "New client connected with FD: " << new_socket << "at port " << server_port << std::endl;

	if (fcntl(new_socket, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
	{
		perror("fcntl nonblock");
		close(new_socket);
		return;
	}

	struct kevent kev;
	EV_SET(&kev, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0)
	{
		perror("Error registering new client socket with kqueue");
	}
	// Add the new_socket to your data structures and update the count of active clients
	active_clients.insert(new_socket);
	this->_currentClientCount++;
}

std::string Server::handleHttpRequest(const std::string& method, const std::string& url, Request& parsedrequest, int client_socket)
{
	// Get the Host header from the request
	(void)method;
	(void)url;
	t_con config;
	HttpRequestHandle	ret;

	config = this->_config.getAllConfig();

	// std::cout << "parsedrequest: " << parsedrequest << std::endl;
	std::string hostHeader = parsedrequest.getHeadersByValue("Host");
	hostHeader = Uti::trim(Uti::splite(hostHeader, ":")[0], " ");
	int server_port = client_to_port_map[client_socket];
	// Store store;

	// Identify the matching server configuration
	t_serverConf* matchedServerConf = NULL;
	for (std::vector<t_serverConf>::iterator it = config.server.begin(); it != config.server.end(); ++it)
	{
		if (it->port == server_port)
		{
			if (hostHeader.empty() || hostHeader.find(it->serverName) != std::string::npos || hostHeader.find("localhost") != std::string::npos)
			{
				matchedServerConf = &(*it);
				break;
			}
		}
	}

	if (!matchedServerConf)
	{
		return Response(400).HttpResponse();
	}
	// std::cout << "------- name -------" << std::endl;
	// std::cout << "Incoming Server Name: " << hostHeader << std::endl;
	// std::cout << "Matched Server Name: " << matchedServerConf->serverName << std::endl;
	// std::cout << "------- name end -------" << std::endl;
	// return  ret.validateMethod(store);

	// std::cout << YELB << "------------ret-------------" << RES << std::endl;
	// std::cout << ret.validateMethod(parsedrequest, config) << std::endl;
	// std::cout << YELB << "----------ret-end-----------" << RES << std::endl;
	// Response restest(200, ret.validateMethod(parsedrequest, config));
	// return ret.validateMethod(parsedrequest, config);
	// return restest.HttpResponse();
	return ret.validateMethod(parsedrequest, config);
	// std::string resp = createSimpleHttpResponse();
	// return (resp);
}

void Server::handleRead(int kq, struct kevent& event)
{
	int eventIdent = event.ident;
	std::cout << "Attempting recv on FD: " << eventIdent << std::endl;
	t_con config;

	int testlen = 0;
	config = this->_config.getAllConfig();

	// Check for EOF event
	if (event.flags & EV_EOF)
	{
		handleClientDisconnection(kq, eventIdent);
		return;
	}

	char buffer[READ_BUFFER_SIZE];
	ssize_t bytesRead = recv(eventIdent, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead < 0)
    {
        // Handle EAGAIN or EWOULDBLOCK
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return;
        }
        perror("recv error");
        return;
    }
    else if (bytesRead == 0)
    {
        // Check if there's any unsent data for the client
        if (!client_write_queues[eventIdent].empty())
        {
            return;
        }
		std::cout << "Client with FD: " << eventIdent << " closed connection" << std::endl;
        handleClientDisconnection(kq, eventIdent);
    }
    else
    {
        buffer[bytesRead] = '\0';
        std::cout << "------- This is buffer -------" << std::endl;
        std::cout << "\n" << buffer << '\n' << bytesRead << "bytes\n" << "\n" << std::endl;
        std::cout << "------- End of buffer -------" << std::endl;

		// Process the received data
		std::string requestData(buffer);
		Request parsedRequest(requestData);
		testlen++;
		if (parsedRequest.getBody().size() > (size_t)config.global.client_body_limit)
		{
			Response res(413);
			send(eventIdent, res.HttpResponse().c_str(), res.size(), 0);
			return;
		}

		std::string httpResponse = handleHttpRequest(parsedRequest.getMethod(), parsedRequest.getUrl(), parsedRequest, eventIdent);

		// Queue the response for the client
		client_write_queues[eventIdent].push(httpResponse);

		// Register for write event to send the response
		struct kevent client_change;
		EV_SET(&client_change, eventIdent, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		kevent(kq, &client_change, 1, NULL, 0, NULL);

		if (got_signal)
		{
			stop();
			return;
		}
	}
}

void Server::handleWrite(int kq, int eventIdent)
{
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
					std::cerr << "Failed to send response to client with FD: " << eventIdent << std::endl;
					// Remove client from active_clients and close its socket
					active_clients.erase(eventIdent);
					client_write_queues.erase(eventIdent);
					// checked_hostnames.erase(eventIdent);
					close(eventIdent);
					this->_currentClientCount--;
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
						std::cout << "encounter 1" << std::endl;
						perror("Error removing write event from kqueue");
					}
				}
			}
		}
		if (write_queue.empty())
		{
			// struct kevent kev;
			// EV_SET(&kev, eventIdent, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
			// if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0)
			// {
			// 	std::cout << "encounter 2" << std::endl;
			// 	perror("Error removing write event from kqueue");
			// }

			// NEW: Check if client has closed its side of the connection
			char temp_buf[1];
			ssize_t checkBytes = recv(eventIdent, temp_buf, sizeof(temp_buf), MSG_PEEK);
			if (checkBytes == 0)
			{
				// Client closed connection, handle cleanup
				active_clients.erase(eventIdent);
				client_write_queues.erase(eventIdent);
				close(eventIdent);
				this->_currentClientCount--;
			}
		}
	}
	else
	{
		std::cerr << "No write queue found for client with FD: " << eventIdent << std::endl;
	}
}

void Server::mainEventLoop(int kq, size_t NUM_SERVERS)
{
	while (this->_running)
	{

		struct kevent events[NUM_SERVERS];
		int numEvents = kevent(kq, NULL, 0, events, NUM_SERVERS, NULL);
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
			if (std::find(_serverSockets.begin(), _serverSockets.end(), event.ident) != _serverSockets.end())
			{
				handleNewClientConnection(kq, event.ident);
			}
			else if (event.filter == EVFILT_READ)
			{
				handleRead(kq, event);
			}
			else if (event.filter == EVFILT_WRITE)
			{
				handleWrite(kq, event.ident);
			}
		}
	}
}

void Server::start()
{
	t_con config;

	config = this->_config.getAllConfig();
	setupSignalHandlers();

	int kq = setupKqueue();

	size_t NUM_SERVERS = config.server.size();
	setupServerSockets(kq, NUM_SERVERS);

	this->_running = true;

	mainEventLoop(kq, NUM_SERVERS);
}
