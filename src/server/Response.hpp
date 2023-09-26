/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:09:37 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/26 11:05:43 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../stdlib.hpp"


class Response
{
	private:
		std::ostringstream	_res;
	public:
		Response();
		Response(int statusCode, const std::string& statusMessage, const std::string& content);
		~Response();
		std::string HttpResponse() const;
};


#endif
