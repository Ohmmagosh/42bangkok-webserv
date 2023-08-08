/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:31 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/08 23:03:54 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../stdlib.hpp"

class Server
{
    public:
        Server(int port);
        // Server(Server const & src);
        ~Server();
        
        int getSocket() const;
        
        void run();
        void setupSocket();
        void handleNewConnection();
        void handleClientData(size_t index);

        class SocketCreationException : public std::exception
        {
            public :
                virtual const char *what() const throw()
                {
                    return ("Error creating socket");
                }
        };

    private:
        int sockfd;
        std::string httpResponse;
        struct sockaddr_in server_addr;
        std::vector<pollfd> fds;
};

#endif