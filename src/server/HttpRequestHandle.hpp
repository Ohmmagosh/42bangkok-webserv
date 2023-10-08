/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:48 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/06 08:45:55 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLE_HPP
# define HTTPREQUESTHANDLE_HPP

# include "../stdlib.hpp"
# include "Response.hpp"

class HttpRequestHandle
{
	private:
		std::string	_method;
		std::string	_path;
		std::string	_cgi_path;
	public:
		HttpRequestHandle();
		HttpRequestHandle(const std::string& method, const std::string& path);
		HttpRequestHandle(const HttpRequestHandle & rhs);
		~HttpRequestHandle();
		HttpRequestHandle &operator=(const HttpRequestHandle & rhs);

		std::string	validateMethod();
		std::string	getMethod();
		std::string	postMethod();
		std::string deleteMethod();
		std::string	readFile(std::stringstream& path);
};

#endif
