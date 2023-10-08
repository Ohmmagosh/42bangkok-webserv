/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:23:37 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/08 18:18:38 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {

}

Request::Request(const std::string& raw_request) {
	std::stringstream	requestStream(raw_request);
	requestStream >> this->_method >> this->_path >> this->_protocol;
	std::string line;
	std::getline(requestStream, line);
	while (std::getline(requestStream, line)) {
		if (line == "\r")
			break;
		size_t colon_p = line.find(':');
		if (colon_p != std::string::npos) {
			std::string key = line.substr(0, colon_p);
			std::string value = line.substr(colon_p + 2, line.size() - colon_p - 3);
			this->_keys.push_back(key);
			this->_headers[key] = value;
		}
	}
	while (std::getline(requestStream, line)) {
		this->_body = line;
	}
}

Request::~Request() {

}

const std::string& Request::getMethod() const {
	return this->_method;
}

const std::string& Request::getPath() const {
	return this->_path;
}

const std::string& Request::getProtocol() const {
	return this->_protocol;
}

const std::string& Request::getBody() const {
	return this->_body;
}

const std::string&	Request::getHeaderValue(const std::string& key) const {
	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
	if (it == this->_headers.end())
		throw HeaderNotFound(key);
	return (it->second);
}

const std::map<std::string, std::string>& Request::getMapHeader() const {
	return this->_headers;
}

int	Request::getHeaderSize() const {
	return this->_headers.size();
}


std::ostream& operator<<(std::ostream& os, const Request& req) {
	os << "Protocol : " << req.getProtocol() << std::endl;
	os << "Method : " << req.getMethod() << std::endl;
	std::map<std::string, std::string>::const_iterator it = req.getMapHeader().begin();
	while (it != req.getMapHeader().end()) {
		os << it->first << " : " << it->second << std::endl;
		it++;
	}
	os << req.getBody() << std::endl;
	return (os);
}
