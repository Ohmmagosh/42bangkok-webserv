/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/23 23:55:51 by psuanpro         ###   ########.fr       */
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

	if (pipe(pipefd) == -1) {
		throw PipeFail();
	}

	pid = fork();
	if (pid == 0) {
		//execve cgi
	} else {
		wait(NULL);

		//send response to client
	}
	return ;
}
