#include "Server.hpp"
#include "TBucket.hpp"
#include "Request.hpp"

volatile sig_atomic_t Server::got_signal = 0;

void Server::signal_handler(int sig)
{
	(void)sig;
    got_signal = 1;
}

Server::Server(int port): port(port), server_fd(0), running(false), rateLimiter(160, 1600)
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

void Server::start()
{
    signal(SIGINT, Server::signal_handler);
    signal(SIGTERM, Server::signal_handler);
    signal(SIGPIPE, SIG_IGN);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		std::cerr << "Failed to create socket." << std::endl;
        stop();
		return;
	}

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        std::cerr << "Failed to set SO_REUSEADDR." << std::endl;
        stop();
        return;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "Failed to set SO_REUSEPORT." << std::endl;
        return;
    }


    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
        std::cerr << "Bind failed." << std::endl;
        stop();
        return;
    }

    if (listen(server_fd, 1024) < 0) 
    {
        std::cerr << "Listen failed." << std::endl;
        stop();
        return;
    }

    int currentClientCount = 0;
    const int MAX_CLIENTS = 1024;
    int kq = kqueue();
    if (kq == -1) {
        std::cerr << "Could not create kqueue" << std::endl;
        stop();
        return;
    }

    struct kevent changeList[1024];
    struct kevent eventList[1024];
    EV_SET(&changeList[0], server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

	running = true;

    while(running) 
    {
        // std::cout 
        std::cout << "work!!" << std::endl;

        int newEvents = kevent(kq, changeList, 1, eventList, 1024, NULL);
        if (newEvents < 0)
        {
            std::cerr << "kevent failed" << std::endl;
            stop();
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
            {
                if (currentClientCount < MAX_CLIENTS)
                {
                    struct sockaddr_in client_address;
                    socklen_t client_addrlen = sizeof(client_address);
                    int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
                    if (new_socket < 0)
                    {
                        std::cerr << "Failed to accept new connection" << std::endl;
                        continue;
                    }

                    if (fcntl(new_socket, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1) 
                    {
                        perror("fcntl");
                        close(new_socket);
                        continue;
                    }

                    if (new_socket >= 0)
                    {
                        struct kevent client_change;
                        EV_SET(&client_change, new_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
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
                        active_clients.insert(new_socket);
                        send(new_socket, httpResponse.c_str(), httpResponse.size(), 0);
                        close(new_socket);
                    }
                }
            }
            else if (event.filter == EVFILT_READ)
            {
                // Handle data from a client or client disconnection
                char buffer[4096];  // 4K buffer for receiving data
                ssize_t bytesRead = recv(event.ident, buffer, sizeof(buffer) - 1, 0);

                // std::cout << "---------\n" << buffer << '\n' << "---------\n" << std::endl;

                if (bytesRead <= 0) 
                {
                    std::cerr << "b < 0" << std::endl;
                    // Client disconnected or error occurred
                    active_clients.erase(event.ident);

                    struct kevent client_change;
                    EV_SET(&client_change, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    if (kevent(kq, &client_change, 1, NULL, 0, NULL) == -1)
                    {
                        std::cerr << "Error: " << strerror(errno) << std::endl;
                        std::cerr << "Could not delete client event from kqueue" << std::endl;
                    }
                    client_write_queues.erase(event.ident);
                    close(event.ident);
                    currentClientCount--;
                } 
                else 
                {
                    if (bytesRead > static_cast<ssize_t>(sizeof(buffer) - 1))
                    {
                        bytesRead = sizeof(buffer) - 1;
                    }
                    if (rateLimiter.consume())
                    {
                        buffer[bytesRead] = '\0';  // Null-terminate the received data
                        std::string request(buffer);

                        // Extract HTTP request details
                        // std::istringstream requestStream(request);
                        Request requestString(request);
                        // ตรงเน้
                        // std::string method, path, protocol;
                        // requestStream >> method >> path >> protocol;

                        std::string httpResponse = handleHttpRequest(requestString.getMethod(), requestString.getPath(), requestString.getProtocol());

                        client_write_queues[event.ident].push(httpResponse);

                        struct kevent client_change;
                        EV_SET(&client_change, event.ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                        kevent(kq, &client_change, 1, NULL, 0, NULL);
                    }
                    else
                    {
                        std::string httpResponse = generateHttpResponse(429, "Too Many Requests", "Rate limit exceeded");
                        send(event.ident, httpResponse.c_str(), httpResponse.size(), MSG_NOSIGNAL);  
                    }

                    if (got_signal)
                    {
                        stop(); // stop the server and clean up
                        break;  // exit the loop
                    }
                }
            }
            else if (event.filter == EVFILT_WRITE) 
            {
                // Your new code to handle write events
                std::queue<std::string>& write_queue = client_write_queues[event.ident];
                if (!write_queue.empty())
                {
                    const std::string& message = write_queue.front();
                    send(event.ident, message.c_str(), message.size(), MSG_NOSIGNAL);
                    write_queue.pop();
                }

                if (write_queue.empty())
                {
                    // Remove write filter if nothing to write
                    struct kevent client_change;
                    EV_SET(&client_change, event.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &client_change, 1, NULL, 0, NULL);
                }
            }
        }
    }
}

void Server::stop()
{
    std::cout << "Stopping server." << std::endl;

    // Close all active client sockets
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
