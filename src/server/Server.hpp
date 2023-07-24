/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:31 by psuanpro          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/07/17 21:44:20 by rchiewli         ###   ########.fr       */
=======
/*   Updated: 2023/07/18 14:53:06 by rchiewli         ###   ########.fr       */
>>>>>>> dfd284bb50ad392d0ee85dbb14c3fe0f4151816a
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