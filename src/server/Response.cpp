/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:09:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/26 11:08:33 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response.hpp"

Response::Response() {
	//default response
}

Response::Response(int statusCode, const std::string& statusMessage, const std::string& content) {
	this->_res << "HTTP/1.1" << statusCode << " " << statusMessage << "\r\n";
	this->_res << "Content-Length:" << content.size() << "\r\n";
	this->_res << "\r\n";
	this->_res << content;
}

Response::~Response() {

}

std::string Response::HttpResponse() const {
	return this->_res.str();
}

