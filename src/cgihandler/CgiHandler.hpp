/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:13 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/29 15:38:53 by psuanpro         ###   ########.fr       */
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
		std::string	executeCgi(StringMatrix& argv, StringMatrix& env);

		std::string getExecuteByUrl(const std::string& url, const t_serverConf& server);
		std::string	getDefaultFileByUrl(const std::string& url, const t_serverConf& serverr);
		std::string	getRootByUrlFromServer(const std::string& url, const t_serverConf& server);
		std::string	getRootDefaultByUrlFromServer(const t_serverConf& server);
		std::string	getExtensionByUrl(const std::string& url, const t_serverConf& server);

		t_detail	getAllLocation(const std::string& url, const t_serverConf& server);
		t_location	getLocationByUrlFromServer(const std::string& url, const t_serverConf& server);

		void		initArgv(const std::string& cgi_exec, const std::string& lc_root, const std::string& def_file);
		void		initArgv(const std::string& cgi_exec, const std::string& def_file);
		void		initEnv(const std::vector<std::string>& env);
		void		initEnv(const std::map<std::string, std::string>& env);
		void		addBackArgv(const std::vector<std::string>& av);

		const std::vector<std::string>&	getArgv() const;
		const std::vector<std::string>&	getEnv() const;


};


#endif
