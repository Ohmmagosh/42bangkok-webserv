/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:23:51 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/30 13:34:15 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Conf.hpp"

Conf::Conf()
{
}

Conf::~Conf()
{
}

bool Conf::parseConfigFile(const std::string &filePath)
{
	std::ifstream file(filePath);
	std::string line;
	std::vector<std::string>	global;
	std::vector<std::string>	servers;
	bool						fglobal = false;
	bool						fservers = false;

	if (!file.is_open())
	{
		return false; // Return false if the file could not be opened
	}

	// parseGlobalSection(file);

	while (std::getline(file, line))
	{
		if (line.find("[GLOBAL]") != std::string::npos) {
			fglobal = true;
		}
		else if (line.find("[SERVERS]") != std::string::npos) {
			fglobal = false;
			fservers = true;
		}
		if (fglobal == true && !line.empty() && !(line.find("#") != std::string::npos))
			global.push_back(line);
		else if (fservers == true && !line.empty() && !(line.find("#") != std::string::npos))
			servers.push_back(line);
	}
	this->setGlobal(global);
	this->setServers(servers);

	// std::cout << "--------------SERVERS--------------" << std::endl;
	// for (size_t i = 0; i < servers.size(); i++) {
	// 	std::cout << "servers["<< i <<"] : " << servers[i] << std::endl;
	// }
	// std::cout << "--------------END--------------" << std::endl;
	return true;
}

const t_globalConf&	Conf::getGlobalConf() const {
	return this->_globalConfig;
}

const std::vector<t_serverConf>&	Conf::getServerConf() const {
	return this->_serverConfigs;
}

const t_con	Conf::getAllConfig() const {
	t_con	ret;

	ret.global = this->_globalConfig;
	ret.server = this->_serverConfigs;
	return ret;
}

void	Conf::setServerHost(t_serverConf *sv, const std::string& line) {
	std::string					empty;
	std::vector<std::string>	sp = Uti::splite(line, ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error : host not found");
		sv->host = empty;
		return ;
	}
	sp[0] = Uti::trim(sp[0], " ;");
	sp[1] = Uti::trim(sp[1], " ;");
	sv->host = sp[1];
	return ;
}

void	Conf::setServerPort(t_serverConf *sv, const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(line, ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error : port not found");
		sv->port = -1;
		return ;
	}
	sp[0] = Uti::trim(sp[0], " ;");
	sp[1] = Uti::trim(sp[1], " ;");
	int port = Uti::strToInt(sp[1]);
	sv->port = port;
	return ;
}

void	Conf::setServerName(t_serverConf *sv, const std::string& line) {
	std::string					empty;
	std::vector<std::string>	sp = Uti::splite(line, ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error : server name not found");
		sv->serverName = empty;
		return ;
	}
	sp[0] = Uti::trim(sp[0], " ;");
	sp[1] = Uti::trim(sp[1], " ;");
	sv->serverName = sp[1];
	return;
}

void	Conf::setServerDefault(t_serverConf *sv, const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(line, ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error : server default not found");
		sv->isDefault = false;
		return ;
	}
	sp[0] = Uti::trim(sp[0], " ;");
	sp[1] = Uti::trim(sp[1], " ;");
	sv->isDefault = (sp[1] == "true" ? true : false);
	return;
}

void	Conf::setServerRoot(t_serverConf *sv, const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(line, ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error : server default not found");
		sv->serverroot = "";
		return ;
	}
	sp[0] = Uti::trim(sp[0], " ;");
	sp[1] = Uti::trim(sp[1], " ;");
	sv->serverroot = sp[1];
	return;
}


std::string	Conf::setServerLocationRoot(const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \t\n;"), ":");
	if (sp.size() < 2) {
		this->_error.push_back("Error: root path not found");
		return "";
	}
	return Uti::trim(sp[1], " \t");
}

std::string	Conf::setServerLocationPath(const std::string& path) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(path, " ;\t"), " ");

	if (sp.size() < 3) {
		this->_error.push_back("Error : location not found");
		return "";
	}
	return sp[1];
}

std::vector<std::string>	Conf::setServerLocationMethod(const std::string& line) {
	std::vector<std::string>	ret;
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \t\n;"), ":");

	if (sp.size() < 2) {
		this->_error.push_back("Error: methods not found or wrong format");
		return ret;
	}
	if(sp[1].find("GET") == std::string::npos && sp[1].find("POST") == std::string::npos && sp[1].find("DELETE") == std::string::npos)
		return ret;
	std::vector<std::string>	sm = Uti::splite(Uti::trim(sp[1], " \t\n[]"),",");
	for (size_t i = 0; i < sm.size(); i++) {
		sm[i] = Uti::trim(sm[i], " ");
	}
	for (size_t j = 0; j < sm.size();j++) {
		ret.push_back(sm[j]);
	}
	return ret;
}

void	Conf::setServerDirListing(t_serverConf *sv, const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \r\n\t;"), ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error : Dirlisting not found or wrong format");
		sv->dirListing = false;
		return ;
	}
	for (size_t i = 0; i < sp.size(); i++) {
		sp[i] = Uti::trim(sp[i], " \t\r\n");
	}
	sv->dirListing =  ((sp[1] == "on") ? true: false);
	return ;
}

std::string	Conf::setServerLocationDefaultFile(const std::string& line) {
	std::vector<std::string>		sp = Uti::splite(Uti::trim(line, " \r\n\t;"), ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error: default file not found or wrong format");
		return "";
	}
	for (size_t i = 0; i < sp.size(); i++) {
		sp[i] = Uti::trim(sp[i], " ");
	}
	return sp[1];
}

bool	Conf::validateFindStr(const std::string& line, const std::string& sfind) {
	if (line.find(sfind) != std::string::npos)
		return true;
	return false;
}

std::string	Conf::setServerLocationCgiExecutable(const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \n\r\t;"), ":");
	if (sp.size() < 2) {
		this->_error.push_back("Error : executable not found");
		return "";
	}
	for (size_t i = 0; i < sp.size();i++) {
		sp[i] = Uti::trim(sp[i], " \t");
	}
	return sp[1];
}

std::string	Conf::setServerLocationCgiExtenstion(const std::string& line) {
	std::cout << CYNB << line << RES << std::endl;
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \n\r\t;"), ":");
	if (sp.size() < 2) {
		this->_error.push_back("Error : exetenstion not found");
		return "";
	}
	for (size_t i = 0; i < sp.size();i++) {
		sp[i] = Uti::trim(sp[i], " \t");
	}
	std::cout << REDB << sp[1] << RES << std::endl;
	return sp[1];
}

t_cgi	Conf::setServerLocationCgi(const std::vector<std::string>& cgi) {
	t_cgi	ret = s_cgi();


	for (size_t i = 1; i < cgi.size();i++) {
		if (this->validateFindStr(cgi[i], "extension")) {
			ret.extension = this->setServerLocationCgiExtenstion(cgi[i]);
		}else if (this->validateFindStr(cgi[i], "executable")) {
			ret.executable = this->setServerLocationCgiExecutable(cgi[i]);
		}
	}
	return ret;
}

bool	Conf::setServerLocationUploadEnable(const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \r\n\t;"), ":");
	if (sp.size() < 2) {
		this->_error.push_back("Error: upload enable not found");
		return false;
	}
	for (size_t i = 0; i < sp.size(); i++) {
		sp[i] = Uti::trim(sp[i], " \t\r\n");
	}
	return (sp[1] == "true" ? true : false);
}

std::string	Conf::setServerLocationCgiUploadPath(const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \r\n\t;"), ":");
	if (sp.size() < 2) {
		this->_error.push_back("Error: upload path not found");
		return "";
	}
	for (size_t i = 0; i < sp.size(); i++) {
		sp[i] = Uti::trim(sp[i], " \t\r\n");
	}
	return sp[1];
}

t_upload	Conf::setServerLocationUpload(const std::vector<std::string>& upl) {
	t_upload	ret = s_upload();
	for (size_t i = 1; i < upl.size();i++) {
		if (this->validateFindStr(upl[i], "enabled")) {
			ret.enabled = this->setServerLocationUploadEnable(upl[i]);
		}else if (this->validateFindStr(upl[i], "save_path")) {
			ret.savePath = this->setServerLocationCgiUploadPath(upl[i]);
		}
	}
	return ret;
}

std::string	Conf::setServerLocationRedirect(const std::string& line) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(line, " \r\n\t;"), ":");
	if (sp.size() != 2) {
		this->_error.push_back("Error: redirect not found");
		return "";
	}
	for (size_t i = 0; i < sp.size(); i++) {
		sp[i] = Uti::trim(sp[i], " \r\n\t");
	}
	return sp[1];
}

t_location	Conf::setServerLocation(const std::vector<std::string>& locations) {
	t_location					ret = s_location();
	t_cgi						cgi = s_cgi();
	bool						b_cgi = false;
	bool						b_upload = false;
	std::vector<std::string>	v_cgi;
	std::vector<std::string>	v_upload;
	int							dept_cgi = 0;
	int							dept_upload = 0;

	for (size_t i = 0; i < locations.size(); i++) {
		if (this->validateFindStr(locations[i],"location")) {
			ret.path = this->setServerLocationPath(locations[i]);
		}
		else if (this->validateFindStr(locations[i], "root")) {
			ret.root = this->setServerLocationRoot(locations[i]);
		}
		else if (this->validateFindStr(locations[i], "method")) {
			ret.method = this->setServerLocationMethod(locations[i]);
		}
		else if (this->validateFindStr(locations[i], "redirect")) {
			ret.redirection = this->setServerLocationRedirect(locations[i]);
		}
		else if (this->validateFindStr(locations[i], "default_file")) {
			ret.defaultFile = this->setServerLocationDefaultFile(locations[i]);
		}
		else if (this->validateFindStr(locations[i], "cgi")) {
			b_cgi = true;
		} else if (this->validateFindStr(locations[i], "upload")) {
			b_upload = true;
		}
		if (b_cgi == true) {
			if (this->validateFindStr(locations[i], "{")) {
				dept_cgi++;
			}else if (this->validateFindStr(locations[i], "}")) {
				dept_cgi--;
			}
			if (dept_cgi != 0)
				v_cgi.push_back(locations[i]);
			if (dept_cgi == 0) {
				ret.cgi = this->setServerLocationCgi(v_cgi);
				v_cgi.clear();
				b_cgi = false;
			}
		}
		if (b_upload == true) {
			if (this->validateFindStr(locations[i], "{")) {
				dept_upload++;
			}else if (this->validateFindStr(locations[i], "}")) {
				dept_upload--;
			}
			if (dept_upload != 0)
				v_upload.push_back(locations[i]);
			if (dept_upload == 0) {
				ret.upload = this->setServerLocationUpload(v_upload);
				v_upload.clear();
				b_upload = false;
			}
		}
	}
	return ret;
}


void	Conf::setServer(const std::vector<std::string>&	servers) {
	bool						blocation = false;
	int							dept = 0;
	std::vector<std::string>	v_location;
	t_serverConf				server = s_serverConf();

	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i].find("host") != std::string::npos)
			this->setServerHost(&server, servers[i]);
		else if (servers[i].find("port") != std::string::npos)
			this->setServerPort(&server, servers[i]);
		else if (servers[i].find("server_name") != std::string::npos)
			this->setServerName(&server, servers[i]);
		else if (servers[i].find("default:") != std::string::npos)
			this->setServerDefault(&server, servers[i]);
		else if (servers[i].find("serverroot:") != std::string::npos)
			this->setServerRoot(&server, servers[i]);
		else if (servers[i].find("directory_listing:") != std::string::npos)
			this->setServerDirListing(&server, server[i]);
		else if (servers[i].find("location") != std::string::npos) {
			blocation = true;
		}
		if (blocation == true) {
			if (servers[i].find("{") != std::string::npos) {
				dept++;
			}
			else if (servers[i].find("}") != std::string::npos)
				dept--;
			if (dept != 0)
				v_location.push_back(servers[i]);
			if (dept == 0) {
				server.location.push_back(this->setServerLocation(v_location));
				blocation = false;
				v_location.clear();
			}
		}
	}
	this->_serverConfigs.push_back(server);
	return ;
}

void	Conf::setServers(const std::vector<std::string>& sv) {
	std::vector<std::string>	servers;
	int		dept = 0;

	for (size_t i = 1; i < sv.size(); i++) {
		if (sv[i].find("SERVERS") != std::string::npos) {
			i++;
			dept++;
		}
		if (sv[i].find("{") != std::string::npos) {
			dept++;
		}
		else if (sv[i].find("}") != std::string::npos) {
			dept--;
		}
		if (dept != 0) {
			servers.push_back(sv[i]);
		}
	}
	std::vector<std::vector<std::string> > server;
	dept = 0;
	std::vector<std::string>	ser;
	for (size_t i = 0; i < servers.size();i++) {
		if (servers[i].find("server ") != std::string::npos) {
			i++;
			dept++;
		}
		if (servers[i].find("{") != std::string::npos) {
			dept++;
		}
		else if (servers[i].find("}") != std::string::npos) {
			dept--;
		}
		if (dept != 0) {
			ser.push_back(servers[i]);
		}
		if (ser.empty() == false && dept == 0) {
			server.push_back(ser);
			ser.clear();
		}
	}
	for (size_t j = 0; j < server.size(); ++j) {
		this->setServer(server[j]);
	}
}

void	Conf::setGlobalDefault(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end) {
	bool										open = false;
	std::vector<std::string>::const_iterator	it;
	for (it = begin; it != end; it++) {
		std::string	buff(*it);
		if (buff.find("{") != std::string::npos)
			open = true;
		if (buff.find("}") != std::string::npos)
			return ;
		std::vector<std::string>	sp = Uti::splite(buff, ":");
		if (sp.size() != 2) {
			std::stringstream	ss;
			ss << "Error : default error pages" << std::endl;
			this->_error.push_back(ss.str());
		} else {
			int	num = Uti::strToInt(Uti::trim(sp[0], " "));
			this->_globalConfig.default_error_pages[num] = Uti::trim(sp[1], " ;");
		}
	}
	return ;
}

void	Conf::setGlobalClient(const std::string& cli) {
	std::vector<std::string>	sp = Uti::splite(cli, ":");
	if (sp.size() < 2) {
		std::stringstream	ss;
		ss << "Error : " << cli << std::endl;
		this->_error.push_back(ss.str());
		return ;
	} else {
		sp[1] = Uti::trim(sp[1], " ;");
		int num = Uti::strToInt(sp[1]);
		if (num < 0) {
			std::stringstream	ss;
			ss << "Error : client size too low" << std::endl;
			this->_error.push_back(ss.str());
			return ;
		}
		this->_globalConfig.client_body_limit = num;
	}
	return ;
}

void	Conf::setGlobal(const std::vector<std::string>& gl) {
	for (size_t i = 0; i < gl.size(); i++) {
		std::string	buff(gl[i]);
		if (buff.find("default_error_pages") != std::string::npos)
			this->setGlobalDefault(gl.begin() + i, gl.end());
		if (buff.find("client_body_limit") != std::string::npos)
			this->setGlobalClient(buff);
	}
	return;
}

void Conf::printGlobalConfig() const
{
	std::cout << "---- Global Configuration ----" << std::endl;

	std::cout << "client_body_limit: " << _globalConfig.client_body_limit << std::endl;

	std::cout << "default_error_pages: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = _globalConfig.default_error_pages.begin(); it != _globalConfig.default_error_pages.end(); ++it)
	{
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	}

	std::cout << "-------------------------------" << std::endl;
}



void Conf::printServerConf() const
{
	std::vector<t_serverConf>::const_iterator serverIt;
	for (serverIt = _serverConfigs.begin(); serverIt != _serverConfigs.end(); ++serverIt)
	{
		std::cout << "Server Config:" << std::endl;
		std::cout << "----------------------" << std::endl;
		std::cout << "Host: " << serverIt->host << std::endl;
		std::cout << "Port: " << serverIt->port << std::endl;
		std::cout << "Server Name: " << serverIt->serverName << std::endl;
		std::cout << "Default: " << ((serverIt->isDefault == true) ? "True" : "False") << std::endl;

		std::cout << "--- Routes ---\n";
		std::vector<t_location>::const_iterator routeIt;
		for (routeIt = serverIt->location.begin(); routeIt != serverIt->location.end(); ++routeIt)
		{
			std::cout << "    Route Path: " << routeIt->path << std::endl;
			std::cout << "    Root: " << routeIt->root << std::endl;
			std::cout << "    Redirect: " << routeIt->redirection << std::endl;
			std::cout << "    Directory Listing: " << (routeIt->dirListing ? "On" : "Off") << std::endl;
			std::cout << "    Default File: " << routeIt->defaultFile << std::endl;

			std::cout << "    CGI:" << std::endl;
			std::cout << "        Extension: " << routeIt->cgi.extension << std::endl;
			std::cout << "        Executable: " << routeIt->cgi.executable << std::endl;

			std::cout << "    Upload:" << std::endl;
			std::cout << "        Enabled: " << (routeIt->upload.enabled ? "True" : "False") << std::endl;
			std::cout << "        Save Path: " << routeIt->upload.savePath << std::endl;
		}
		std::cout << "----------------------" << std::endl
				  << std::endl;
	}
}

