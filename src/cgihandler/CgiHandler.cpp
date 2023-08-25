/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/24 01:11:09 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"


CgiHandler::CgiHandler() {

}

CgiHandler::~CgiHandler() {

}

void	CgiHandler::run() {
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1) {
		throw PipeFail();
	}

	pid = fork();
	if (pid == 0) {
		close(pipefd[0]);
		// execve();
		//execve cgi
		close(pipefd[1]);
	} else {
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		//send response to client
		close(pipefd[0]);
	}
	return ;
}
