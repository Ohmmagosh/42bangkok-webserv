#include "Server.hpp"

Server* serverInstance = NULL;

Server::Server(int port): keepRunning(true)
{
    serverInstance = this;

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

    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    struct sigaction saInt;
    saInt.sa_handler = sigint_handler;
    saInt.sa_flags = SA_RESTART; 
    sigemptyset(&saInt.sa_mask);
    if (sigaction(SIGINT, &saInt, NULL) == -1) {
        perror("sigaction SIGINT");
        exit(1);
    }

    struct sigaction saTerm;
    saTerm.sa_handler = sigterm_handler;
    saTerm.sa_flags = SA_RESTART; 
    sigemptyset(&saTerm.sa_mask);
    if (sigaction(SIGTERM, &saTerm, NULL) == -1) {
        perror("sigaction SIGTERM");
        exit(1);
    }

    this->setupSocket();
}

void Server::run()
{
    while (keepRunning) 
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0)
        {
            if (errno == EINTR)
            {
                // poll was interrupted by a signal, continue with the loop
                continue;
            }
            perror("poll");
            break;
        }

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].fd == sockfd && fds[i].revents & POLLIN)
            {
                handleNewConnection();
            }
            else if (fds[i].revents & POLLIN)
            {
                handleClientData(i);
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

void Server::handleClientData(size_t index) 
{
    (void)index;
    // Code to handle data from a connected client goes here...
}

void Server::handleClient(int client_sockfd)
{

    if (fcntl(client_sockfd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
    {
        perror("fcntl");
        close(client_sockfd);
        return;
    }
    
    // Here, you can implement a loop to continuously read data from the client
    // until they disconnect or you can simply read once depending on your design.

    char buffer[4096];
    int bytes_read = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
    {
        perror("recv");
        close(client_sockfd);
        return;
    }

    buffer[bytes_read] = '\0';  // Null-terminate the received data
    std::cout << "Received: " << buffer << std::endl;

    // parser แก้พาร์ทนี้ไปรีเชคเออเรอ กับรายละเอียดให้ด้วย แยกเป็นฟังชั่นให้ด้วย
    std::istringstream requestStream(buffer);
    std::string requestMethod, requestPath, httpVersion;
    requestStream >> requestMethod >> requestPath >> httpVersion;
    // ถึงตรงนี้

    // landing page and response
    std::string httpResponse;
    if (requestPath == "/")
    {
        std::string htmlContent;
        try 
        {
            htmlContent = readFile("./src/server/index.html"); // แก้เเแลนดิ้งให้เป็นตามconfig
            httpResponse = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n\r\n";
            httpResponse += htmlContent;
        }
        catch (const std::exception& e)
        {
            httpResponse = "HTTP/1.1 500 Internal Server Error\r\n"
                           "Content-Type: text/html\r\n\r\n"
                           "<html><body><h1>500 Internal Server Error</h1></body></html>";
        }
    } 
    else
    {
        httpResponse = "HTTP/1.1 404 Not Found\r\n"
                       "Content-Type: text/html\r\n\r\n"
                       "<html><body><h1>404 Not Found</h1></body></html>";
    }
    // ถึงตรงนี้

    // Here, you can handle the client's request and send back a response
    // For now, let's just echo back the received data
    send(client_sockfd, httpResponse.c_str(), httpResponse.length(), 0);

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

std::string Server::readFile(const std::string& filename) 
{
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string content, line;
    while (getline(file, line)) {
        content += line + "\n";
    }

    file.close();
    return content;
}

void Server::stop()
{
    keepRunning = false;
}

void Server::print_error(const char* prefix)
{
    std::cerr << prefix << ": " << strerror(errno) << std::endl;
}

void sigchld_handler(int s) 
{
    (void)s;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void sigint_handler(int signo)
{
    (void)signo;
    serverInstance->stop();
}

void sigterm_handler(int signo) 
{
    (void)signo;
    serverInstance->stop();
}