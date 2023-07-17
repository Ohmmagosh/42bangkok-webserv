/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <Marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:31 by psuanpro          #+#    #+#             */
/*   Updated: 2023/07/17 21:05:06 by psuanpro         ###   ########.fr       */
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

        Server &	operator=(Server const & rhs);

    private:

};
