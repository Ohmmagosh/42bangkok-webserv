/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:09:37 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/20 01:36:54 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../stdlib.hpp"


class Response
{
	private:
		std::stringstream					_res;
		std::map<size_t, std::string>	_statusMessage;
	public:
		Response();
		Response(size_t statusCode, const std::string& content);
		Response(size_t statusCode);
		~Response();
		std::string	HttpResponse() const;
		std::string errResponse() const;
		std::string getStatusMessage(size_t statusCode);
		const char* resStr() const;
		void	initMessage();
		size_t		size() const;
};


#endif
