/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:48 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/26 05:39:30 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLE_HPP
# define HTTPREQUESTHANDLE_HPP

# include "../stdlib.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Store.hpp"
# include "../parsing/Conf.hpp"
# include "../cgihandler/CgiHandler.hpp"


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
		// HttpRequestHandle(const std::string& method);
		// HttpRequestHandle(const HttpRequestHandle & rhs);
		// HttpRequestHandle(const Request& req, const std::string& cgi_path);
		// HttpRequestHandle(const Request& req, const std::string& cgi_path, const std::vector<ServerConfig>& serverConfigs);
		~HttpRequestHandle();
		HttpRequestHandle &operator=(const HttpRequestHandle & rhs);

		const std::string&	validateMethod(const Request& req, const t_con& config);
		bool						validateMethodAllow(std::vector<std::string> method, const std::string& vmethod);
		bool						validateUrlAllow(const std::string& url, const t_con& config, const std::string& vmethod);
		bool						validateCgi();

		const std::string&	getMethodRoute(const std::string& url, const Request& req, const t_con& config);
		const std::string&	getMethod(const Request& req, const t_con& config);
		int							getPortFromRequest(Request req);
		const std::string&	postMethod(const Request& req, Store *st);
		const std::string&	deleteMethod(const Request& req);
		std::string			readFile(std::stringstream& path);
};

#endif
