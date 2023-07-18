/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:31 by psuanpro          #+#    #+#             */
/*   Updated: 2023/07/18 14:53:06 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../stdlib.hpp"

class Server
{
    public:
        Server();
        Server(Server const & src);
        ~Server();
        
        int startsocket();
        void shutdown();

        Server &	operator=(Server const & rhs);

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
        sockaddr_in server_addr;
};

#endif