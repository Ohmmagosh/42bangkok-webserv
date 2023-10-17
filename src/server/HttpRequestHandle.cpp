/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/18 03:33:51 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"

HttpRequestHandle::HttpRequestHandle()
{
}

HttpRequestHandle::HttpRequestHandle(const std::string &method): _cgi_path("./src/server"), _method(method)
{
	//should from config
}

HttpRequestHandle::HttpRequestHandle(const Request& req, const std::string& cgi_path): _req(req), _cgi_path(cgi_path){

}

HttpRequestHandle::HttpRequestHandle(const HttpRequestHandle &rhs)
{
	*this = rhs;
}

HttpRequestHandle::~HttpRequestHandle()
{
}

HttpRequestHandle &HttpRequestHandle::operator=(const HttpRequestHandle &rhs)
{
	if (this != &rhs)
	{
		// this->_method = rhs._method;
		// this->_path = rhs._path;
	}
	return *this;
}

std::string HttpRequestHandle::validateMethod(Store *st)
{
	if (this->_req.getMethod() == "GET" && st->getMultiPart() == false) {
		return this->getMethod(this->_req);
	}
	else if (this->_req.getMethod() == "POST" || st->getMultiPart()) {
		return this->postMethod(this->_req, st);
	}
	else if (this->_req.getMethod() == "DELETE" && st->getMultiPart() == false) {
		return this->deleteMethod(this->_req);
	}
	else {
		Response error(501, "Not Implement", "This method is not implemented");
		return error.HttpResponse();
	}
}

std::string HttpRequestHandle::getMethod(const Request& req)
{
	std::stringstream path;
	if (!this->_cgi_path.empty() || this->_cgi_path != "") {
		if (req.getPath() == "/" || req.getPath() == "/index.html")
			path << this->_cgi_path << "/index.html";
		else
			path << this->_cgi_path << req.getPath();
		return this->readFile(path);
	}
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}

std::string HttpRequestHandle::postMethod(const Request& req, Store *st)
{
	Console::log(0, "hello");
	std::cout << "-------------------" << std::endl;
	std::string		content = req.getHeaderValue("Content-Type");
	size_t			pos = content.find("multipart/form-data;");
	std::cout << pos << std::endl;
	if (pos != std::string::npos && st->getMultiPart() == false) {
		st->setMultiPart(true);
	}
	Console::log(1, "HELLOOOOOOOOOOO POST\n");
	std::cout << "-------------------" << std::endl;
	// (void)st;
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}


//get path and delete file
std::string HttpRequestHandle::deleteMethod(const Request& req)
{
	(void)req;
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}

std::string HttpRequestHandle::readFile(std::stringstream& path) {
	std::ifstream file(path.str(), std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::cout << content << std::endl;
		Console::modeMsg(1, content + "\n");
		file.close();
		Response res(200, "Ok", content);
		return res.HttpResponse();
	}
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}
