/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/27 02:09:47 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"


HttpRequestHandle::HttpRequestHandle()
{
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


bool	HttpRequestHandle::validateUrlAllow(const std::string& url, const t_con& config, const std::string& vmethod) {

	for (size_t i = 0; i < config.server.size(); i++) {
		for (size_t j = 0; j < config.server[i].location.size(); j++) {
			if (config.server[i].location[j].path == url) {
				if (this->validateMethodAllow(config.server[i].location[j].method, vmethod))
					return true;
			}
		}
	}
	return false;
}

bool	HttpRequestHandle::validateMethodAllow(std::vector<std::string> method, const std::string& vmethod) {
	for (size_t i = 0; i < method.size(); i++) {
		if (method[i] == vmethod)
			return true;
	}
	return false;
}

bool	HttpRequestHandle::validateCgi(const std::string& url,const t_con& config) {
	CgiHandler cgi;
	size_t	find_url = url.find(cgi.getExtensionByUrl(url, config.server));
	if (find_url != std::string::npos)
		return true;
	return false;
}

std::string	HttpRequestHandle::getMethodRoute(const std::string& url, const Request& req, const t_con& config) {
	CgiHandler cgi;

	if (this->validateCgi(url, config)) {
		// if (url == "/") {
		// 	cgi.initArgv(cgi.getExecuteByUrl(url, config.server),cgi.getRootByUrl(url, config.server) + "main.py" );
		// 	StringMatrix	argv(cgi.getArgv());
		// 	return cgi.executeCgi(argv);
		// }
		return Response(404, "<h1>do not have cgi</h1>").HttpResponse();
	}else {
		if (url == "/" || url == "/index.html") {
			//return default file
			std::string	path = cgi.getRootByUrl(url, config.server);
			path += "/";
			path += cgi.getDefaultFileByUrl(url, config.server);
			std::string file = File(path).getContent();
			if (file.empty()) {
				return Response(404, "<h1>Not Found</h1>").HttpResponse();
			}
			return Response(200, file).HttpResponse();
		}
		else {
			std::string path = cgi.getRootByUrl(url, config.server);
			path += req.getUrl();
			std::string	file = File(path).getContent();
			std::stringstream ss;
			ss << "<h1> Not found path : " << path << "</h1>" << std::endl;
			if (file.empty()) {
				return Response(404, ss.str()).HttpResponse();
			}
			return Response(200, file).HttpResponse();
		}
	}
	return	Response(404).HttpResponse();
}

t_detail	HttpRequestHandle::validateHostRequestAndGetServer(const Request& req, const t_con& config) {
	std::string host = req.getHeadersByValue("Host");
	if (host.find("localhost") != std::string::npos) {
		std::string	port = host.substr(host.find_last_of(':') + 1);
		for (size_t i = 0; i < config.server.size(); i++) {
			if (config.server[i].port == atoi(port.c_str()))
				return Detail(true, port, config.server[i]).getDetailStruct();
		}
		return Detail(false).getDetailStruct();
	}
	else {
		for (size_t i = 0; i < config.server.size(); i++) {
			if (config.server[i].serverName == host)
				return Detail(true, config.server[i]);
		}
	}
	return Detail(false).getDetailStruct();
}

std::string	HttpRequestHandle::getMethod(const Request& req, const t_con& config) {

	if (this->validateUrlAllow(req.getUrl(), config, "GET")) {
		return this->getMethodRoute(req.getUrl(), req, config);
	}
	return Response(404, "<h1>Hello getmethod </h1>").HttpResponse();
}

std::string	HttpRequestHandle::validateMethod(const Request& req,const t_con& config) {

	if (req.getMethod() == "GET") {
		return this->getMethod(req, config);
	}
	else if (req.getMethod() == "POST") {
		return Response(200,"<div>HELLO POST</div>").HttpResponse();
	}
	else if (req.getMethod() == "DELETE") {
		return Response(200, "<div>HELLO Delete</div>").HttpResponse();
	}
	return Response(404, "<h1>HELLO not found</h1>").HttpResponse();
}


int HttpRequestHandle::getPortFromRequest(Request req)
{
	std::string hostHeader = req.getHeadersByValue("Host");
	std::string::size_type colonPos = hostHeader.find_last_of(':');
	if (colonPos != std::string::npos)
	{
		std::string portStr = hostHeader.substr(colonPos + 1);
		std::istringstream iss(portStr);
		int port;
		iss >> port;
		if (!iss.fail())
		{
			return port;
		}
	}
	return -1; // Return -1 or any other value to indicate that the port wasn't found
}


