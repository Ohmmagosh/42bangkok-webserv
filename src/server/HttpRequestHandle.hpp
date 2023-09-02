/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:48 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/02 19:16:15 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLE_HPP
# define HTTPREQUESTHANDLE_HPP

# include "../stdlib.hpp"
# include "Request.hpp"

enum Method {GET, POST, PUT, DELETE, UNKNOW};
class HttpRequestHandle
{
	private:
	public:
		HttpRequestHandle();
		HttpRequestHandle(const Request & request);
		~HttpRequestHandle();

		HttpRequestHandle &operator=(const HttpRequestHandle & rhs);
		Method	mapRequestToMethod(const std::string & method);
		void	getMethod();
		void	postMethod();
		void	putMethod();
		void	deleteMethod();

};

#endif
