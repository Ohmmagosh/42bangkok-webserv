/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:48 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/18 01:27:41 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLE_HPP
# define HTTPREQUESTHANDLE_HPP

# include "../stdlib.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Store.hpp"

class HttpRequestHandle
{
	private:
		Request		_req;
		std::string	_cgi_path;
		std::string	_method;
	public:
		HttpRequestHandle();
		HttpRequestHandle(const std::string& method);
		HttpRequestHandle(const HttpRequestHandle & rhs);
		HttpRequestHandle(const Request& req, const std::string& cgi_path);
		~HttpRequestHandle();
		HttpRequestHandle &operator=(const HttpRequestHandle & rhs);

		std::string	validateMethod(Store *st);
		std::string	getMethod(const Request& req);
		std::string	postMethod(const Request& req, Store *st);
		std::string deleteMethod(const Request& req);
		std::string	readFile(std::stringstream& path);
};

#endif
