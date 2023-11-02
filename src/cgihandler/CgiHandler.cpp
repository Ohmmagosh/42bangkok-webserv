/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/11/02 19:19:15 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

pid_t child_pid = -1;

CgiHandler::CgiHandler() {

}

CgiHandler::~CgiHandler() {

}

static void timeout_handler(int signum)
{
	(void)signum;
    if (child_pid > 0) {
        kill(child_pid, SIGKILL);  // Forcefully kill child process
        child_pid = -1;  // Reset PID
    }
}

std::string	CgiHandler::executeCgi(StringMatrix& argv, StringMatrix& env) {

	int	pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("error pipe");
		exit(EXIT_FAILURE);
	}
	signal(SIGALRM, timeout_handler);
	alarm(30);
	pid_t	pid = fork();
	if (pid == 0) { //child
		close(pipefd[0]); //close read
		dup2(pipefd[1], STDOUT_FILENO);
		if (execve(argv[0], argv.getStr(), env.getStr()) == -1)
			perror("execve");
		exit(EXIT_FAILURE);
	}else { //parent
		child_pid = pid;
		close(pipefd[1]); //close write
		std::string res;
		char	buff[4096];
		ssize_t byteRead;
		while ((byteRead = read(pipefd[0], buff, sizeof(buff))) > 0) {
			res.append(buff, byteRead);
		}
		close(pipefd[0]);
		int status;
		waitpid(child_pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL) {
			res = Response(500).HttpResponse();
		}
		return res;
	}
}

t_detail	CgiHandler::getAllLocation(const std::string& url, const t_serverConf& server) {
	for (size_t i = 0; i < server.location.size(); i++) {
		if (url == server.location[i].path)
			return Detail(true, server.location[i]).getDetailStruct();
	}
	return Detail(false).getDetailStruct();
}


std::string	CgiHandler::getRootDefaultByUrlFromServer(const t_serverConf& server) {
	if (server.isDefault)
		return server.serverroot;
	return "";
}

t_location	CgiHandler::getLocationByUrlFromServer(const std::string& url, const t_serverConf& server) {
	for (size_t i = 0; i < server.location.size(); i++) {
		if (server.location[i].path == url)
			return server.location[i];
	}
	t_location empty;
	return empty;
}

std::string	CgiHandler::getDefaultFileByUrl(const std::string& url, const t_serverConf& server) {
	return this->getLocationByUrlFromServer(url, server).defaultFile;
}

std::string	CgiHandler::getRootByUrlFromServer(const std::string& url, const t_serverConf& server) {
	return this->getLocationByUrlFromServer(url, server).root;
}

std::string	CgiHandler::getExecuteByUrl(const std::string& url, const t_serverConf& server) {
	return this->getLocationByUrlFromServer(url, server).cgi.executable;
}
std::string	CgiHandler::getExtensionByUrl(const std::string& url, const t_serverConf& server) {
	for (size_t i = 0; i < server.location.size(); i++) {
		if (url == server.location[i].path) {
			return server.location[i].cgi.extension;
		}
	}
	return "";
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

void	CgiHandler::initEnv(const std::vector<std::string>& env) {
	for (size_t i = 0; i < env.size(); i++) {
		this->_env.push_back(env[i]);
	}
	return ;
}

void	CgiHandler::initEnv(const std::map<std::string, std::string>& env) {
	std::map<std::string, std::string>::const_iterator it = env.begin();
	for (;it != env.end(); it++) {
		std::stringstream	ss;
		ss << it->first << "=" << it->second;
		this->_env.push_back(ss.str());
	}
	return ;
}
