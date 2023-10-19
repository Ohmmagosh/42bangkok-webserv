/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:23:37 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/19 07:35:34 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// Request::Request() {

// }

// Request::Request(const std::string& raw_request) {
// 	std::stringstream	requestStream(raw_request);
// 	std::string line;

// 	this->setRawReqToVector(raw_request);
// 	this->setHeaderAndBody(this->_req);
// }

// Request::~Request() {

// }


// const std::string& Request::getMethod() const {
// 	return this->_method;
// }

// const std::string& Request::getPath() const {
// 	return this->_path;
// }

// const std::string& Request::getProtocol() const {
// 	return this->_protocol;
// }

// const std::string& Request::getBody() const {
// 	return this->_body;
// }

// std::string	Request::getHeaderValue(const std::string& key) const {
// 	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
// 	std::string		empty;
// 	if (it == this->_headers.end())
// 		return "";
// 	return (it->second);
// }

// const std::map<std::string, std::string>& Request::getMapHeader() const {
// 	return this->_headers;
// }

// int	Request::getHeaderSize() const {
// 	return this->_headers.size();
// }

// void	Request::setHeader(const std::string& header)
// {
// 	std::string			line;
// 	std::stringstream	ss(header);

// 	if (this->isMethod(header))
// 	{
// 		ss >> this->_method >> this->_path >> this->_protocol;
// 		if (this->_path.find("?") != std::string::npos) {
// 			std::vector<std::string>	sp = Utility::splite(this->_path, "?");
// 			this->setParams(sp[1]);
// 		}
// 		while (std::getline(ss,line))
// 		{
// 			size_t colon_p = line.find(':');
// 			if (colon_p != std::string::npos)
// 			{
// 				std::string key = line.substr(0, colon_p);
// 				std::string value = line.substr(colon_p + 2, line.size() - colon_p - 3);
// 				this->_keys.push_back(key);
// 				this->_headers[key] = value;
// 			}
// 		}
// 	}
// 	return ;
// }

// void	Request::setRawReqToVector(const std::string& raw_request) {
// 	std::string		delim = "\r\n\r\n";
// 	this->_req = Utility::splite(raw_request, delim);
// }

// int	Request::getReqVectorSize() const{
// 	return (this->_req.size());
// }

// const std::vector<std::string>&	Request::getVectorReq() const {
// 	return (this->_req);
// }

// std::ostream& operator<<(std::ostream& os, const Request& req) {
// 	if (!req.getProtocol().empty())
// 		os << "Protocol : " << req.getProtocol() << std::endl;
// 	if (!req.getMethod().empty()) {
// 		os << "Method : " << req.getMethod() << std::endl;
// 	}
// 	os << "-------------HEADER--------------" << std::endl;
// 	std::map<std::string, std::string>::const_iterator it = req.getMapHeader().begin();
// 	while (it != req.getMapHeader().end()) {
// 		os << it->first << " : " << it->second << std::endl;
// 		it++;
// 	}
// 	if (!req.getBody().empty()) {
// 		os << "-----------BODY----------" << std::endl;
// 		os << req.getBody();
// 	}
// 	return (os);
// }

// std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec) {
// 	std::vector<std::string>::const_iterator it;

// 	for (it = vec.begin(); it != vec.end(); it++) {
// 		os << "--------------------------fdsafdsaff-----------------------------\n";
// 		os << *it << std::endl;
// 		os << "--------------------------ffasdfdsaf-----------------------------\n";
// 	}
// 	return (os);
// }

// void	Request::setHeaderAndBody(const std::vector<std::string>& req) {
// 	if (req.size() == 2) {
// 		this->setHeader(req[0]);
// 		this->setBody(req[1]);
// 	}
// 	else if (req.size() == 1) {
// 		if (this->isMethod(req[0])) {
// 			this->setHeader(req[0]);
// 		}
// 	}
// }

// bool	Request::isMethod(const std::string& req_header) {
// 	size_t	get = req_header.find("GET");
// 	size_t	post = req_header.find("POST");
// 	size_t	del = req_header.find("DELETE");

// 	if (get != std::string::npos || post != std::string::npos || del != std::string::npos)
// 		return true;
// 	return false;
// }

// void	Request::setBody(const std::string& body) {
// 	this->_body = body;
// }

// void	Request::setParams(const std::string& params) {
// 	std::vector<std::string>	parameters;
// 	if (params.find("&") == std::string::npos)
// 		return ;
// 	parameters = Utility::splite(params, "&");
// 	for (size_t i = 0; i < parameters.size(); i++) {
// 		std::vector<std::string>	param = Utility::splite(parameters[i], "=");
// 		if (param.empty())
// 			return ;
// 		this->_params[param[0]] = param[1];
// 	}
// 	return ;
// }

Request::Request() {

}

Request::Request(const std::string& raw_req) {
	this->setHeaderAndBody(raw_req);
}

Request::~Request() {

}

//GETTER
std::string	Request::getMethod() const {
	return this->_method;
}


std::string	Request::getUrl() const {
	return this->_url;
}

std::string	Request::getVersion() const {
	return this->_version;
}

std::string	Request::getBoundary() const {
	return this->_boundary;
}

const std::map<std::string, std::string>&	Request::getHeaderC() const {
	return this->_headers;
}

const std::vector<std::string>&	Request::getHeaderNoC() const {
	return this->_headers_no_c;
}

std::string	Request::getHeadersByValue(const std::string& key) {
	return this->_headers[key];
}

std::string	Request::getBody() const {
	return this->_body;
}

const std::map<std::string, std::string>&	Request::getQueryUrl() const {
	return this->_query_url;
}

std::string	Request::getQueryUrlByValue(const std::string& val){
	return this->_query_url[val];
}

//SETTER
void	Request::setMethod(const std::string& method) {
	this->_method = method;
}

void	Request::setUrl(const std::string& url) {
	this->_url = url;
}

void	Request::setVersion(const std::string& version) {
	this->_version = version;
}


void	Request::setBoundary(const std::string& boundary) {
	this->_boundary = boundary;
}

void	Request::setBody(const std::string& body) {
	this->_body = body;
}

void	Request::setAllHeader(const std::string& header) {
	std::string	buff;
	std::stringstream	ss(header);

	while(std::getline(ss, buff)) {
		if (this->validateHeaderMethod(buff))
			this->setHeaderMethod(buff);
		else
			this->validateHeadersCandNoC(buff);
	}
	if (this->validateBoundary())
		this->setBoundaryFromContent(this->getHeadersByValue("Content-Type"));
	if (this->validateParams())
		this->setQueryUrl();
}

void	Request::setAllBody(const std::string& body) {
	this->setBody(body);
}

void	Request::setHeaderAndBody(const std::string& raw_req) {
	std::vector<std::string>	sp = Utility::splite(raw_req, "\r\n\r\n");
	this->setAllHeader(sp[0]);
	this->setAllBody(sp[1]);
}

void	Request::setQueryUrl() {
	std::string	url = this->getUrl();
	if (url.empty())
		return ;
	if (url.find("?")) {
		std::vector<std::string>	sp = Utility::splite(url, "?");
		if (sp[1].empty())
			return ;
		std::vector<std::string>	sa = Utility::splite(sp[1], "&");
		for (size_t i = 0; i < sa.size(); i++) {
			std::vector<std::string> se = Utility::splite(sa[i], "=");
			this->_query_url[se[0]] = se[1];
		}
	}
	return ;
}

void	Request::setHeaderC(const std::string& line) {
	std::vector<std::string>	sp;
	std::stringstream			ss;
	sp = Utility::spliteHeader(line, ":");
	this->_headers[sp[0]] = sp[1];
}

void	Request::setHeaderNoC(const std::string& line) {
	this->_headers_no_c.push_back(line);
}

void	Request::setHeaderMethod(const std::string& line) {
	std::string	method;
	std::string	url;
	std::string	version;
	std::stringstream	ss(line);

	ss >> method >> url >> version;
	this->setMethod(method);
	this->setUrl(url);
	this->setVersion(version);
	return ;
}

void	Request::setBoundaryFromContent(const std::string& content) {
	std::vector<std::string>	sp = Utility::splite(Utility::trim(content, " "), " ");
	std::vector<std::string>	bp = Utility::splite(sp[1], "=");
	this->setBoundary(bp[1]);
}

// HELPER
bool	Request::validateBoundary() {
	std::string	content_type = this->getHeadersByValue("Content-Type");
	if (content_type.find("boundary") != std::string::npos)
		return true;
	return false;
}

bool	Request::validateMultiPart() {
	std::string	content_type = this->getHeadersByValue("Content-Type");
	if (content_type.find("multipart/form-data") != std::string::npos)
		return true;
	return false;
}


void	Request::validateHeadersCandNoC(const std::string& line) {
	std::vector<std::string>	sp;
	if (line.find(":") != std::string::npos) {
		sp = Utility::splite(line, ":");
		this->setHeaderC(line);
	}else {
		this->setHeaderNoC(line);
	}
}

bool	Request::validateHeaderMethod(const std::string& line) {
	if (line.find("GET") != std::string::npos)
		return true;
	else if (line.find("POST") != std::string::npos)
		return true;
	else if (line.find("DELETE") != std::string::npos)
		return true;
	return false;
}

bool	Request::validateBoundaryBody() const {
	std::string		body = this->getBody();
	std::string			boundary = this->getBoundary();
	if (body.empty() || boundary.empty())
		return false;
	std::stringstream	ss(body);
	std::string			buff;
	while(std::getline(ss, buff)) {
		if (Utility::trim(buff, "-\r\n").compare(Utility::trim(boundary, "-\r\n")) == 0)
			return true;
	}
	return false;
}

bool	Request::validateParams() {
	std::string	url = this->getUrl();
	if (url.empty())
		return false;
	if (url.find("?"))
		return true;
	return false;
}

//OSTREAM
std::ostream&	operator<<(std::ostream& os, const Request& req) {
	std::map<std::string, std::string>::const_iterator	it = req.getHeaderC().begin();
	std::map<std::string, std::string>					query = req.getQueryUrl();
	os << "----------------------ostream---------------------" << std::endl;
	os << "---------------------[HEADER]---------------------" << std::endl;
	os << "Method : " << req.getMethod() << " Url : " << req.getUrl() << " Version : " << req.getVersion() << std::endl;
	for (size_t i = 0; i < req.getHeaderNoC().size(); i++) {
		os << req.getHeaderNoC()[i] << std::endl;
	}
	while (it != req.getHeaderC().end()) {
		os << it->first << ":" << it->second << std::endl;
		it++;
	}
	os << "boundary : " << req.getBoundary() << std::endl;
	if (!query.empty()) {
		std::map<std::string, std::string>::const_iterator	qit = query.begin();
		while (qit != query.end()) {
			os << "[QUERY] : " << qit->first << " = " << qit->second << std::endl;
			qit++;
		}
	}
	else
		os << "[NO QUERY PARAMS]" << std::endl;
	os << "-------------------[END HEADER]-------------------" << std::endl;
	os << "---------------------[BODY]---------------------" << std::endl;
	if (req.getBody().empty())
		os << "BODY IS EMPTY" << std::endl;
	else
		os << req.getBody() << std::endl;
	os << "---------------------[END]---------------------" << std::endl;
	os << "--------------------end ostream-------------------" << std::endl;
	return os;
}
