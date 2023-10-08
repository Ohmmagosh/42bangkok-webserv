/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/08 18:36:59 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"

HttpRequestHandle::HttpRequestHandle()
{
}

HttpRequestHandle::HttpRequestHandle(const std::string &method, const std::string &path) : _method(method), _path(path)
{
	//should from config
	this->_cgi_path = "./src/server";
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
		this->_method = rhs._method;
		this->_path = rhs._path;
	}
	return *this;
}
//if cgi_path == null
//openfile and return with contents string
//else
//use execute cgi
std::string HttpRequestHandle::validateMethod()
{
	if (this->_method == "GET") {
		return this->getMethod();
	}
	else if (this->_method == "POST") {
		return this->postMethod();
	}
	else if (this->_method == "DELETE") {
		return this->deleteMethod();
	}
	else {
		Response error(501, "Not Implement", "This method is not implemented");
		return error.HttpResponse();
	}
}

std::string HttpRequestHandle::getMethod()
{
	std::stringstream path;
	if (!this->_cgi_path.empty() || this->_cgi_path != "") {
		if (this->_path == "/" || this->_path == "/index.html")
		{
			this->_path = "/index.html";
			path << this->_cgi_path << this->_path;
		}else {
			path << this->_cgi_path << this->_path;
		}
		return this->readFile(path);
	}
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}

std::string HttpRequestHandle::postMethod()
{
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}


//get path and delete file
std::string HttpRequestHandle::deleteMethod()
{
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}

std::string HttpRequestHandle::readFile(std::stringstream& path) {
	std::ifstream file(path.str(), std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::cout << content << std::endl;
		file.close();
		Response res(200, "Ok", content);
		return res.HttpResponse();
	}
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}
