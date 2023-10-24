/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:48 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/24 23:51:00 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLE_HPP
# define HTTPREQUESTHANDLE_HPP

# include "../stdlib.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Store.hpp"
# include "../parsing/Conf.hpp"

struct ServerConfig
{
    int port;
    std::string name;
    std::string landingPagePath;
};

class HttpRequestHandle
{
	private:
		Request						_req;
		std::string					_cgi_path;
		std::string					_method;
		std::vector<ServerConfig>	_serverConfigs;
	public:
		HttpRequestHandle();
		HttpRequestHandle(const std::string& method);
		HttpRequestHandle(const HttpRequestHandle & rhs);
		HttpRequestHandle(const Request& req, const std::string& cgi_path);
		HttpRequestHandle(const Request& req, const std::string& cgi_path, const std::vector<ServerConfig>& serverConfigs);
		~HttpRequestHandle();
		HttpRequestHandle &operator=(const HttpRequestHandle & rhs);

		std::string	validateMethod(const Request& req, const t_con& config);
		bool		validateMethodAllowRoute(const std::string url, const t_con& config);
		std::string	getMethod(const Request& req, const t_con& config);
		std::string	postMethod(const Request& req, Store *st);
		std::string deleteMethod(const Request& req);
		std::string	readFile(std::stringstream& path);
		int	getPortFromRequest(Request req);
};

#endif
