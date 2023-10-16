#ifndef SERVER_HPP
# define SERVER_HPP

# include "../stdlib.hpp"
# include "TBucket.hpp"
# include "HttpRequestHandle.hpp"
# include "Store.hpp"
# include "Response.hpp"

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
		// struct kevent events;
        int		server_fd;
        bool	running;
		int 	MAX_CLIENTS;
        std::set<int> active_clients;
		std::vector<int> serverSockets;
		// std::vector<int> ports;
		std::map<int, std::string> client_buffers;
		std::vector<std::pair<int, std::string> > serverPortNamePairs;
		TBucket rateLimiter;
		int currentClientCount;
		Store store;

		size_t MAX_BODY_SIZE;
		std::string dlpath;
		std::string dlname;

		static void signal_handler(int sig);
		void setNonBlocking();
		void closeActiveClients();
		void closeServerSocket();
		void setupSignalHandlers();
		int  setupKqueue();
		void setupServerSockets(int kq, size_t NUM_SERVERS);
		void mainEventLoop(int kq, int& currentClientCount, size_t NUM_SERVERS);
		void handleEventError(struct kevent& event);
		void handleNewClientConnection(int kq, int& currentClientCount, int eventIdent);
		void handleClientRead(int kq, int eventIdent);
		void handleClientWrite(int kq, int eventIdent);
		std::string extractHostHeader(const std::string& request);
		std::string handleHttpRequest(const std::string& method, const std::string& path, const std::string& protocol, const std::string& hostHeader);
		std::string handleFileDownloadRequest();
};

#endif
