/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/26 22:39:57 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() {

}

CgiHandler::~CgiHandler() {

}

std::string	CgiHandler::executeCgi(StringMatrix& argv) {
	int	pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("error pipe");
		exit(EXIT_FAILURE);
	}
	pid_t	pid = fork();
	if (pid == 0) { //child
		close(pipefd[0]); //close read
		dup2(pipefd[1], STDOUT_FILENO);
		if (execve(argv[0], argv.getStr(), NULL) == -1)
			perror("execve");
		exit(EXIT_FAILURE);
	}else { //parent
		close(pipefd[1]); //close write
		std::string res;
		char	buff[4096];
		ssize_t byteRead;
		while ((byteRead = read(pipefd[0], buff, sizeof(buff))) > 0) {
			res.append(buff, byteRead);
		}
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
		return res;
	}
}

std::string CgiHandler::getExecuteByUrl(const std::string& url, const std::vector<t_serverConf>& server) {
	for (size_t i = 0; i < server.size(); i++) {
		for (size_t j = 0; j < server[i].location.size(); j++) {
			if (server[i].location[j].path == url) {
				return server[i].location[j].cgi.executable;
			}
		}
	}
	return "";
}

t_location CgiHandler::getLocationByUrl(const std::string& url, const std::vector<t_serverConf>& server) {
	for (size_t i = 0; i < server.size(); i++) {
		for (size_t j = 0; j < server[i].location.size(); j++) {
			if (server[i].location[j].path == url) {
				return server[i].location[j];
			}
		}
	}
	t_location empty = s_location();
	return empty;
}

std::string	CgiHandler::getDefaultFileByUrl(const std::string& url, const std::vector<t_serverConf>& server) {
	t_location	location = this->getLocationByUrl(url, server);

	return location.defaultFile;
}


std::string CgiHandler::getRootByUrl(const std::string& url, const std::vector<t_serverConf>& server) {
	t_location	location = this->getLocationByUrl(url, server);
	return location.root;
}

std::string CgiHandler::getExtensionByUrl(const std::string& url, const std::vector<t_serverConf>& server) {
	t_location	location = this->getLocationByUrl(url, server);
	return location.cgi.extension;
}

void	CgiHandler::addBackArgv(const std::vector<std::string>& av) {
	for (size_t i = 0; i < av.size(); i++) {
		this->_argv.push_back(av[i]);
	}
	return ;
}

void	CgiHandler::initArgv(const std::string& cgi_exec, const std::string& lc_root, const std::string& def_file) {
	this->_argv.push_back(cgi_exec);
	this->_argv.push_back(lc_root);
	this->_argv.push_back(def_file);
	return ;
}

void	CgiHandler::initArgv(const std::string& cgi_exec, const std::string& def_file) {
	this->_argv.push_back(cgi_exec);
	this->_argv.push_back(def_file);
	return ;
}

const std::vector<std::string>& CgiHandler::getArgv() const {
	return this->_argv;
}
const std::vector<std::string>& CgiHandler::getEnv() const {
	return this->_env;
}


// char** CgiHandler::initEnv(const Request & req) {
// 	char** res = new char*[req.getHeaderSize()];
// 	std::map<std::string, std::string>::const_iterator	it = req.getMapHeader().begin();
// 	int i = 0;
// 	while(it != req.getMapHeader().end() && i < req.getHeaderSize()) {
// 		std::stringstream	ss;
// 		ss << it->first << "=" << it->second;
// 		res[i] = strdup(ss.str().c_str());
// 		it++;
// 		i++;
// 	}
// 	res[req.getHeaderSize()] = NULL;
// }
