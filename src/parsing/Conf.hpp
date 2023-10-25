/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:23:39 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/25 01:40:46 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_HPP
# define CONF_HPP

# include "../stdlib.hpp"

typedef struct	s_upload
{
	bool                        enabled;
	std::string                 savePath;

	s_upload()
	{
		enabled  = false;
		savePath = "";
	}
}				t_upload;

typedef struct	s_cgi
{
	std::string				extension;
	std::string				executable;

	s_cgi()
	{
		extension   = "";
		executable  = "";
	}
}				t_cgi;

typedef struct	s_location
{
	std::string					path;
	std::string					root;
	std::vector<std::string>	method;
	std::string					redirection;
	bool						dirListing;
	std::string					defaultFile;
	t_cgi						cgi;
	t_upload					upload;

	s_location()
	{
		path        =   "";
		root        =   "";
		method.clear();
		redirection =   "";
		dirListing  =   false;
		defaultFile =   "";
		cgi         =   s_cgi();
		upload      =   s_upload();
	}
}				t_location;

typedef struct	s_serverConf
{
	std::string					host;
	int							port;
	std::string					serverName;
	bool						isDefault;
	std::vector<t_location>		location;

	s_serverConf()
	{
		host        =   "";
		port        =   0;
		serverName  =   "";
		isDefault   =   false;
		location.clear();
	}
}				t_serverConf;

typedef struct	s_globalConf
{
	std::map<int, std::string>	default_error_pages;
	int									client_body_limit;

	s_globalConf()
	{
		default_error_pages.clear();
		client_body_limit = 0;
	}
}				t_globalConf;

typedef struct 	s_con
{
	t_globalConf				global;
	std::vector<t_serverConf>	server;
}				t_con;

class Conf
{
	public:
		Conf();
		~Conf();
		bool						parseConfigFile(const std::string& filePath);
		void						parseGlobalSection(std::ifstream& file);
		void						parseServerSection(std::ifstream& file);
		void						parseCgiSection(std::ifstream& file);
		void						parseRouteSection(std::ifstream& file);


		//GETTER

		const t_con								getAllConfig() const;
		const t_globalConf&						getGlobalConf() const;
		const std::vector<t_serverConf>&		getServerConf() const;
		//HELPER
		bool						validateFindStr(const std::string& line, const std::string& sfind);

		//SETTER
		void						setGlobal(const std::vector<std::string>& gl);
		void						setServers(const std::vector<std::string>& sv);
		void						setServer(const std::vector<std::string>& sv);
		void						setGlobalDefault(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end);
		void						setGlobalClient(const std::string& cli);
		void						setServerHost(t_serverConf *sv, const std::string& line);
		void						setServerPort(t_serverConf *sv, const std::string& line);
		void						setServerName(t_serverConf *sv, const std::string& line);
		void						setServerDefault(t_serverConf *sv, const std::string& line);
		t_location					setServerLocation(const std::vector<std::string>& locations);
		std::string					setServerLocationPath(const std::string& line);
		std::string					setServerLocationRoot(const std::string& line);
		std::vector<std::string>	setServerLocationMethod(const std::string& line);
		std::string					setServerLocationRedirect(const std::string& line);
		bool						setServerLocationDirlisting(const std::string& line);
		std::string					setServerLocationDefaultFile(const std::string& line);
		void						setServerRouteLocation(t_location *lc, const std::vector<std::string>& line);
		std::string					setServerLocationCgiExtenstion(const std::string& line);
		std::string					setServerLocationCgiExecutable(const std::string& line);
		t_cgi						setServerLocationCgi(const std::vector<std::string>& cgi);
		t_upload					setServerLocationUpload(const std::vector<std::string>& upl);
		bool						setServerLocationUploadEnable(const std::string& line);
		std::string					setServerLocationCgiUploadPath(const std::string& line);

		// //getter
		// std::map<int, std::string>  getDefaultErrorPages() const;
		// int                         getClientBodyLimit() const;
		// std::string                 getHost(size_t serverIndex) const;
		// int                         getPort(size_t serverIndex) const;
		// std::string                 getServerName(size_t serverIndex) const;
		// bool                        getDefault(size_t serverIndex) const;

		// //printer
		void                        printGlobalConfig() const;
		void                        printServerConf() const;
		// void                        printDefaultErrorPages() const;
		// void                        printHost(size_t serverIndex) const;
		// void                        printPort(size_t serverIndex) const;
		// void                        printServerName(size_t serverIndex) const;
		// void                        printDefault(size_t serverIndex) const;

	private:
		t_globalConf				_globalConfig;
		std::vector<t_serverConf>   _serverConfigs;
		std::vector<std::string>	_error;

		// t_upload                     parseUploadSection(std::ifstream& configFile);
		// t_cgi                        parseCgiSection(std::ifstream& configFile);
		// std::vector<t_routes>        parseRoutesSection(std::ifstream& configFile);



};

#endif
