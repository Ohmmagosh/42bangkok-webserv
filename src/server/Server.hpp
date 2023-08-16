/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:31 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/11 16:04:07 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../stdlib.hpp"

class Server
{
	public:
		Server(int port);
		~Server();
		void start();
		void stop();

	private:
		int port;
		int server_fd;
		bool running;
		struct sockaddr_in address;
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
