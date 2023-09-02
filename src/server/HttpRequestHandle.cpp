/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/02 19:21:08 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"

HttpRequestHandle::HttpRequestHandle() {

}

HttpRequestHandle::HttpRequestHandle( const Request & request) {
	int	method_num;

	method_num = this->mapRequestToMethod(request.getMethod());
	switch (method_num) {
		case 0:
			//something
			break;
		case 1:
			//something
			break;
		case 2:
			//something
			break;
		case 3:
			//something
			break;
		case 4:
			//something
			break;
	}
}

Method	HttpRequestHandle::mapRequestToMethod(const std::string & method) {
	if (method == "GET") return GET;
	if (method == "POST") return POST;
	if (method == "PUT") return PUT;
	if (method == "DELETE") return DELETE;
	return UNKNOW;
}

HttpRequestHandle::~HttpRequestHandle() {

}

HttpRequestHandle& HttpRequestHandle::operator=(const HttpRequestHandle & rhs) {
	// if (this != &rhs) {  // Check for self-assignment
	//     this->someMemberVariable = rhs.someMemberVariable;
	//     // Copy other member variables
	// }
	(void)rhs;
	return *this;
}
