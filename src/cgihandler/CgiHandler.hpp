/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:13 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/26 05:32:55 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../stdlib.hpp"
# include "../server/Request.hpp"
# include "../parsing/Conf.hpp"
#include <string>

class CgiHandler {
	private:
		std::vector<std::string>	_argv;
		std::vector<std::string>	_env;
	public:
		CgiHandler();
		~CgiHandler();
		std::string	executeCgi(StringMatrix& argv);
		// char** initEnv(const Request & req);

		std::string getExecuteByUrl(const std::string& url, const std::vector<t_serverConf>& server);
		std::string	getDefaultFileByUrl(const std::string& url, const std::vector<t_serverConf>& server);
		t_location	getLocationByUrl(const std::string& url, const std::vector<t_serverConf>& server);
		std::string	getRootByUrl(const std::string& url, const std::vector<t_serverConf>& server);
		std::string	getExtensionByUrl(const std::string& url, const std::vector<t_serverConf>& server);

		void		initArgv(const std::string& cgi_exec, const std::string& lc_root, const std::string& def_file);
		void		initArgv(const std::string& cgi_exec, const std::string& def_file);
		void		addBackArgv(const std::vector<std::string>& av);

		const std::vector<std::string>&	getArgv() const;
		const std::vector<std::string>&	getEnv() const;


};


#endif
