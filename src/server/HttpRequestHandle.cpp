/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/27 16:13:14 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"

HttpRequestHandle::HttpRequestHandle()
{
}

HttpRequestHandle::HttpRequestHandle(const std::string &method, const std::string &path) : _method(method), _path(path)
{
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

std::string HttpRequestHandle::validateMethod()
{
	if (this->_method == "GET")
	{
		return this->getMethod();
	}
	else if (this->_method == "PUT")
	{
		return this->putMethod();
	}
	else if (this->_method == "DELETE")
	{
		return this->deleteMethod();
	}
	else
	{
		Response error(501, "Not Implement", "This method is not implemented");
		return error.HttpResponse();
	}
}

std::string HttpRequestHandle::getMethod()
{
	if (this->_path == "/" || this->_path == "/index.html")
	{
		std::stringstream ss;
		this->_path = "/index.html";
		ss << this->_cgi_path << this->_path;
		std::cout << "ss : " << ss.str() << std::endl;
		std::ifstream file(ss.str(), std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			std::cout << content << std::endl;
			file.close();
			Response res(200, "Ok", content);
			return res.HttpResponse();
		}
	}
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}

std::string HttpRequestHandle::putMethod()
{
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}

std::string HttpRequestHandle::deleteMethod()
{
	Response err404(404, "not Found", "file not found");
	return err404.HttpResponse();
}
