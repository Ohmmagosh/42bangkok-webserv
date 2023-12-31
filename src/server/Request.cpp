/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:23:37 by psuanpro          #+#    #+#             */
/*   Updated: 2023/11/02 11:29:18 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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

std::string Request::getContent() const {
	return this->_content;
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
	if (url.find("?") != std::string::npos) {
		std::vector<std::string>	sp = Uti::splite(url, "?");
		this->_url = sp[0];
		std::string	query;
		query += "?";
		query += sp[1];
		this->setQueryUrl(query);
		return ;
	}
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
	else
		this->_boundary = "";
}

void	Request::setAllBody(const std::string& body) {
	this->setBody(body);
}

size_t	Request::lenLine(const std::string& content) {
	std::istringstream	sc(content);
	std::string			line;
	size_t				len = 0;
	while (std::getline(sc, line)) {
		len++;
	}
	return len;
}

std::string	Request::deleteBoundaryFromContent(const std::string& content) {
	std::string line;
	std::string bd = this->getBoundary();
	size_t		lenl = this->lenLine(content);
	size_t		len = 0;
	std::istringstream	sc(content);
	std::string	ret;

	if (bd.empty())
		return "";
	while (std::getline(sc, line)) {
		if (len == lenl - 1) {
			std::string	tr = Uti::trim(line, "-\r\n");
			std::string tb = Uti::trim(bd, "-\n\r");
			if (tb == tr)
				break;
			return "";
		}
		ret += line;
		ret += '\n';
		len++;
	}
	return ret;
}

void	Request::setAllContentMultipart(const std::string& content) {
	this->_content = this->deleteBoundaryFromContent(content);
}

void	Request::setAddHeaderFromBody(const std::string& content) {
	std::istringstream	sc(content);
	std::string			line;

	while (std::getline(sc, line)) {
		std::vector<std::string>	sp = Uti::splite(line, ":");
		if (sp.size() == 1)
			this->setHeaderNoC(line);
		else
			this->setHeaderC(line);
	}
}

void	Request::setHeaderAndBody(const std::string& raw_req) {
	std::vector<std::string>	sp = Uti::splite(raw_req, "\r\n\r\n");
	this->setAllHeader(sp[0]);
	this->setAllBody(sp[1]);
	if (sp.size() == 3) {
		this->setAllContentMultipart(sp[2]);
		this->setAddHeaderFromBody(sp[1]);
	}
	return ;
}

void	Request::setQueryUrl(const std::string& url) {
	if (url.empty())
		return ;
	if (url.find("?") != std::string::npos) {
		std::vector<std::string>	sp = Uti::splite(url, "?");
		if (sp[1].empty())
			return ;
		std::vector<std::string>	sa = Uti::splite(sp[1], "&");
		for (size_t i = 0; i < sa.size(); i++) {
			std::vector<std::string> se = Uti::splite(sa[i], "=");
			this->_query_url[se[0]] = se[1];
		}
	}
	return ;
}

void	Request::setHeaderC(const std::string& line) {
	std::vector<std::string>	sp;
	std::stringstream			ss;
	sp = Uti::spliteHeader(line, ":");
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
	std::vector<std::string>	sp = Uti::splite(Uti::trim(content, " "), " ");
	std::vector<std::string>	bp = Uti::splite(sp[1], "=");
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
		sp = Uti::splite(line, ":");
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
		if (Uti::trim(buff, "-\r\n").compare(Uti::trim(boundary, "-\r\n")) == 0)
			return true;
	}
	return false;
}

bool	Request::validateParams() {
	std::string	url = this->getUrl();
	if (url.empty())
		return false;
	if (url.find("?") != std::string::npos)
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
	os << "--------------------[CONTENT]---------------------" << std::endl;
	if (req.getContent().empty())
		os << "CONTENT IS EMPTY" << std::endl;
	else
		os << req.getContent() << std::endl;
	os << "-------------------[END CONTENT]-------------------" << std::endl;
	os << "--------------------end ostream-------------------" << std::endl;
	return os;
}
