/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:31 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/11 02:00:07 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../stdlib.hpp"
# include "TBucket.hpp"

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

extern volatile sig_atomic_t got_signal;

class Server
{
	public:
		Server(int port);
		~Server();
		void start();
		void stop();
		std::map<int, std::queue<std::string> > client_write_queues;
		static volatile sig_atomic_t got_signal;

    private:
        int port;
		// std::vector<int>
        int server_fd;
        bool running;
        struct sockaddr_in address;
		static void signal_handler(int sig);
        std::set<int> active_clients;
		TokenBucket rateLimiter;
        // int addrlen;

		// parsing data
		// std::string method;
		// std::string path;
		// std::string protocol;
		//

		void setNonBlocking();
		// bool parseHttpRequest(const std::string &request, std::string &method, std::string &path, std::string &protocol);
		std::string handleHttpRequest(const std::string &method, const std::string &path, const std::string &protocol);
		std::string generateHttpResponse(int statusCode, const std::string &statusMessage, const std::string &content);
};

#endif