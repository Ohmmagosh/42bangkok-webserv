/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:09:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/11/03 20:10:39 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {
	//default response
}

Response::Response(size_t statusCode, const std::string& content) {
	this->initMessage();
	this->_res << "HTTP/1.1 " << statusCode << " " << this->getStatusMessage(statusCode) << "\r\n";
	this->_res << "Content-Length: " << content.size() << "\r\n";
	this->_res << "\r\n";
	this->_res << content;
}

Response::Response(size_t statusCode, const std::string& redirect, const std::string& content) {
	this->initMessage();

	std::string	error;
	std::map<int, std::string>::iterator it = g_conf.default_error_pages.begin();
	for (;it != g_conf.default_error_pages.end();it++) {
		if (static_cast<size_t>(it->first) == statusCode)
			error = File(it->second).getContent();
	}
	this->_res << "HTTP/1.1 " << statusCode << " " << this->getStatusMessage(statusCode) << "\r\n";
	if (!redirect.empty())
		this->_res << "Location: " << redirect << "\r\n";
	this->_res << "\r\n";
	if (!content.empty())
		this->_res << error;
}

Response::Response(size_t statusCode) {
	this->initMessage();
	std::cout << "HELLO" << std::endl;
	std::string	error;
	std::map<int , std::string>::iterator it = g_conf.default_error_pages.begin();
	for (;it != g_conf.default_error_pages.end();it++) {
		if (static_cast<size_t>(it->first) == statusCode)
			error = File(it->second).getContent();
	}
	this->_res << "HTTP/1.1 " << statusCode << " " << this->getStatusMessage(statusCode) << "\r\n";
	this->_res << "\r\n";
	std::cout << error << std::endl;
	if (!error.empty())
		this->_res << error;
}

Response::~Response() {

}

void	Response::initMessage() {
	this->_statusMessage[100] = "Continue";
	this->_statusMessage[101] = "Switching Protocols";
	this->_statusMessage[102] = "Processing";
	this->_statusMessage[103] = "Early Hints";
	this->_statusMessage[200] = "OK";
	this->_statusMessage[201] = "Created";
	this->_statusMessage[202] = "Accepted";
	this->_statusMessage[203] = "Non-Authoritative Information";
	this->_statusMessage[204] = "No Content";
	this->_statusMessage[205] = "Reset Content";
	this->_statusMessage[206] = "Partial Content";
	this->_statusMessage[207] = "Multi-Status";
	this->_statusMessage[208] = "Already Reported";
	this->_statusMessage[226] = "IM Used";
	this->_statusMessage[300] = "Multiple Choices";
	this->_statusMessage[301] = "Moved Permanently";
	this->_statusMessage[302] = "Found";
	this->_statusMessage[303] = "See Other";
	this->_statusMessage[304] = "Not Modified";
	this->_statusMessage[305] = "Use Proxy";
	this->_statusMessage[306] = "Switch Proxy";
	this->_statusMessage[307] = "Temporary Redirect";
	this->_statusMessage[308] = "Permanent Redirect";
	this->_statusMessage[400] = "Bad Request";
	this->_statusMessage[401] = "Unauthorized";
	this->_statusMessage[402] = "Payment Required";
	this->_statusMessage[403] = "Forbidden";
	this->_statusMessage[404] = "Not Found";
	this->_statusMessage[405] = "Method Not Allowed";
	this->_statusMessage[406] = "Not Acceptable";
	this->_statusMessage[407] = "Proxy Authentication Required";
	this->_statusMessage[408] = "Request Timeout";
	this->_statusMessage[409] = "Conflict";
	this->_statusMessage[410] = "Gone";
	this->_statusMessage[411] = "Length Required";
	this->_statusMessage[412] = "Precondition Failed";
	this->_statusMessage[413] = "Payload Too Large";
	this->_statusMessage[414] = "URI Too Long";
	this->_statusMessage[415] = "Unsupported Media Type";
	this->_statusMessage[416] = "Range Not Satisfiable";
	this->_statusMessage[417] = "Expectation Failed";
	this->_statusMessage[418] = "I'm a teapot";
	this->_statusMessage[421] = "Misdirected Request";
	this->_statusMessage[422] = "Unprocessable Entity";
	this->_statusMessage[423] = "Locked";
	this->_statusMessage[424] = "Failed Dependency";
	this->_statusMessage[425] = "Too Early";
	this->_statusMessage[426] = "Upgrade Required";
	this->_statusMessage[428] = "Precondition Required";
	this->_statusMessage[429] = "Too Many Requests";
	this->_statusMessage[431] = "Request Header Fields Too Large";
	this->_statusMessage[451] = "Unavailable For Legal Reasons";
	this->_statusMessage[500] = "Internal Server Error";
	this->_statusMessage[501] = "Not Implemented";
	this->_statusMessage[502] = "Bad Gateway";
	this->_statusMessage[503] = "Service Unavailable";
	this->_statusMessage[504] = "Gateway Timeout";
	this->_statusMessage[505] = "HTTP Version Not Supported";
	this->_statusMessage[506] = "Variant Also Negotiates";
	this->_statusMessage[507] = "Insufficient Storage";
	this->_statusMessage[508] = "Loop Detected";
	this->_statusMessage[510] = "Not Extended";
	this->_statusMessage[511] = "Network Authentication Required";
}

std::string	Response::HttpResponse() const {
	return this->_res.str();
}

size_t	Response::size() const {
	std::string	str(this->_res.str());
	return (str.size());
}

std::string Response::getStatusMessage(size_t statusCode) {
	return this->_statusMessage[statusCode];
}


