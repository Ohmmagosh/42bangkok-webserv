/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:23:37 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/17 00:23:55 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {

}

Request::Request(const std::string& raw_request) {
	std::stringstream	requestStream(raw_request);
	std::string line;

	this->setRawReqToVector(raw_request);
	this->setHeaderAndBody(this->_req);
	// std::map<std::string, std::string>::iterator it = this->_headers.begin();
	// while(it != this->_headers.end()) {
	// 	std::stringstream	ss;
	// 	ss << it->first << " : " << it->second << std::endl;
	// 	// Console::modeMsg(0, ss.str());
	// 	// it++;
	// }
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

void	Request::setHeader(const std::string& header) {
	std::string			line;
	std::stringstream	ss(header);
	if (this->isMethod(header)) {
		ss >> this->_method >> this->_path >> this->_protocol;
	}
	while (std::getline(ss,line)) {
		size_t colon_p = line.find(':');
		if (colon_p != std::string::npos) {
			std::string key = line.substr(0, colon_p);
			std::string value = line.substr(colon_p + 2, line.size() - colon_p - 3);
			// Console::cmMsg(1, value, "test", "end-end");
			this->_keys.push_back(key);
			this->_headers[key] = value;
		}
	}
}


std::vector<std::string>	Request::splite(const std::string& req, const std::string& delim) {
	std::vector<std::string>	ret;
	size_t						start = 0;
	size_t						end = req.find(delim);

	while (end != std::string::npos) {
		ret.push_back(req.substr(start , end - start));
		start = end + delim.length();
		end = req.find(delim, start);
	}
	ret.push_back(req.substr(start, end ));
	return ret;

}

void	Request::setRawReqToVector(const std::string& raw_request) {
	std::string		delim = "\r\n\r\n";
	this->_req = this->splite(raw_request, delim);
}

int	Request::getReqVectorSize() const{
	return (this->_req.size());
}

const std::vector<std::string>&	Request::getVectorReq() const {
	return (this->_req);
}

std::ostream& operator<<(std::ostream& os, const Request& req) {
	if (!req.getProtocol().empty())
		os << "Protocol : " << req.getProtocol() << std::endl;
	if (!req.getMethod().empty()) {
		os << "Method : " << req.getMethod() << std::endl;
	}
	os << "-------------HEADER--------------" << std::endl;
	std::map<std::string, std::string>::const_iterator it = req.getMapHeader().begin();
	while (it != req.getMapHeader().end()) {
		os << it->first << " : " << it->second << std::endl;
		it++;
	}
	if (!req.getBody().empty()) {
		os << "-----------BODY----------" << std::endl;
		os << req.getBody();
	}
	return (os);
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec) {
	std::vector<std::string>::const_iterator it;

	for (it = vec.begin(); it != vec.end(); it++) {
		os << "--------------------------fdsafdsaff-----------------------------\n";
		os << *it << std::endl;
		os << "--------------------------ffasdfdsaf-----------------------------\n";
	}
	return (os);
}

void	Request::setHeaderAndBody(const std::vector<std::string>& req) {
	if (req.size() == 2) {
		this->setHeader(req[0]);
		this->setBody(req[1]);
	}
	else if (req.size() == 1) {
		if (this->isMethod(req[0])) {
			this->setHeader(req[0]);
		}
	}
}

bool	Request::isMethod(const std::string& req_header) {
	size_t	get = req_header.find("GET");
	size_t	post = req_header.find("POST");
	size_t	del = req_header.find("DELETE");

	if (get != std::string::npos || post != std::string::npos || del != std::string::npos)
		return true;
	return false;
}

void	Request::setBody(const std::string& body) {
	this->_body = body;
}
