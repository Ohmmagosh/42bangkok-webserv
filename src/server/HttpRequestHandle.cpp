/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/29 17:14:14 by psuanpro         ###   ########.fr       */
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

	}
	return *this;
}

bool	HttpRequestHandle::validateUrlAllow(const std::string& url, const t_serverConf& server, const std::string& vmethod) {
	for (size_t i = 0; i < server.location.size(); i++) {
		if (server.location[i].path == url) {
			if (this->validateMethodAllow(server.location[i].method, vmethod))
				return true;
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

bool	HttpRequestHandle::validateCgi(const std::string& url, const t_serverConf& server) {
	std::string	extention = CgiHandler().getExtensionByUrl(url, server);
	if (extention.empty())
		return false;
	if (url.find(extention) != std::string::npos) {
		return true;
	}
	return false;
}

std::string	HttpRequestHandle::getMethodRoute(const std::string& url, const Request& req, const t_serverConf& server) {
	CgiHandler helper;

	if (this->validateCgi(url, server)) {
		CgiHandler cgi;
		std::string	path_file;
		path_file += cgi.getRootByUrlFromServer(url, server);
		path_file += url;
		cgi.initArgv(cgi.getExecuteByUrl(url, server), path_file);
		StringMatrix	argv(cgi.getArgv());
		StringMatrix	env(req.getQueryUrl());
		return Response(200,cgi.executeCgi(argv, env)).HttpResponse();
	}else {
		if (url == "/" || url == "/index.html") {
			std::string	path = helper.getRootByUrlFromServer(url, server);
			path += "/";
			path += helper.getDefaultFileByUrl(url, server);
			std::string file = File(path).getContent();
			if (file.empty()) {
				return Response(404, "<h1>Not Found</h1>").HttpResponse();
			}
			return Response(200, file).HttpResponse();
		}
		else {
			t_detail	loc = helper.getAllLocation(url, server);

			if (loc.status) {
				if (this->validateMethodAllow(loc.location.method, "GET"))
					return Response(405).HttpResponse();
			}
			std::string path = helper.getRootByUrlFromServer(url, server);
			if (path.empty()) {
				path = helper.getRootDefaultByUrlFromServer(server);
			}
			path += req.getUrl();
			std::string	file = File(path).getContent();
			std::stringstream ss;
			ss << "<h1> Path Not found : " << path << "</h1>" << std::endl;
			if (file.empty()) {
				return Response(404, ss.str()).HttpResponse();
			}
			return Response(200, file).HttpResponse();
		}
	}
	return	Response(404, "<h1> Get not found </h1>").HttpResponse();
}

t_detail	HttpRequestHandle::validateHostRequestAndGetServer(Request& req, const t_con& config) {

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
				return Detail(true, config.server[i]).getDetailStruct();
		}
	}
	return Detail(false).getDetailStruct();
}

std::string	HttpRequestHandle::getMethod(const Request& req, const t_serverConf& server) {

	// if (this->validateUrlAllow(req.getUrl(), server, "GET")) {

	return this->getMethodRoute(req.getUrl(), req, server);

	// }
	// return Response(404, "<h1>Hello getmethod </h1>").HttpResponse();
}

std::string	HttpRequestHandle::validateMethod(const Request& req,const t_con& config) {

	// (void)config;
	t_detail tmp = this->validateHostRequestAndGetServer(const_cast<Request&>(req), config);
	if (tmp.status) {
		if (req.getMethod() == "GET") {
			return this->getMethod(req, tmp.server);
			// return Response(200,"<h1>HELLO GET</h1>").HttpResponse();
		}
		else if (req.getMethod() == "POST") {
			return Response(200,"<div>HELLO POST</div>").HttpResponse();
		}
		else if (req.getMethod() == "DELETE") {
			return Response(200, "<div>HELLO Delete</div>").HttpResponse();
		}
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


