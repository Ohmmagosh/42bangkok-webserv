#ifndef SERVER_HPP
# define SERVER_HPP

# include "../stdlib.hpp"
# include "TBucket.hpp"
# include "HttpRequestHandle.hpp"

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

extern volatile sig_atomic_t got_signal;

class Server
{
	public:
		Server();
		~Server();
		void start();
		void stop();
		std::map<int, std::queue<std::string> > client_write_queues;
		static volatile sig_atomic_t got_signal;

    private:
        int server_fd;
        bool running;
		int MAX_CLIENTS;
		static void signal_handler(int sig);
        std::set<int> active_clients;
		std::vector<int> serverSockets;
		// std::vector<int> ports;
		std::vector<std::pair<int, std::string> > serverPortNamePairs;
		TokenBucket rateLimiter;

		void setNonBlocking();
		std::string extractHostHeader(const std::string& request);
		std::string handleHttpRequest(const std::string& method, const std::string& path, const std::string& protocol, const std::string& hostHeader);
		std::string generateHttpResponse(int statusCode, const std::string &statusMessage, const std::string &content);
};

#endif
