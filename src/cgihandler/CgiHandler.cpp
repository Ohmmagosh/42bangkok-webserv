/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/07 13:03:40 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"


CgiHandler::CgiHandler() {

}

CgiHandler::~CgiHandler() {

}
// argv = {/usr/lib/python3}
//
//
//
//


std::string	CgiHandler::executeCgi() {
	char path[] = "/usr/bin/python3";
	char file[] = "hello.py";
	char *argv[] = {path,file , NULL};
	char *env[] = {"hello=world", NULL};
	int	pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("error pipe");
		exit(EXIT_FAILURE);
	}
	pid_t	pid = fork();
	if (pid == 0) { //child
		close(pipefd[0]); //close read
		dup2(pipefd[1], STDOUT_FILENO);
		if (execve(argv[0], argv, env) == -1)
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


// char** CgiHandler::initEnv(const Request & req) {
// 	char** res = new char*[req.getSize()];


// }
