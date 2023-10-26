/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/26 05:40:15 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"


HttpRequestHandle::HttpRequestHandle()
{
}

// HttpRequestHandle::HttpRequestHandle(const std::string &method): _cgi_path("./src/server"), _method(method)
// {
// 	//should from config
// }

// HttpRequestHandle::HttpRequestHandle(const Request& req, const std::string& cgi_path, const std::vector<ServerConfig>& serverConfigs): _req(req), _cgi_path(cgi_path), _serverConfigs(serverConfigs)
// {
// }

// HttpRequestHandle::HttpRequestHandle(const Request& req, const std::string& cgi_path): _req(req), _cgi_path(cgi_path){

// }

// HttpRequestHandle::HttpRequestHandle(const HttpRequestHandle &rhs)
// {
// 	*this = rhs;
// }

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

bool	HttpRequestHandle::validateCgi() {

}
//Method GET
const std::string&	HttpRequestHandle::getMethodRoute(const std::string& url, const Request& req, const t_con& config) {
	CgiHandler cgi;
	(void)req;

	if (url == "/") {
		cgi.initArgv(cgi.getExecuteByUrl(url, config.server),cgi.getRootByUrl(url, config.server) + "main.py" );
		StringMatrix	argv(cgi.getArgv());
		return cgi.executeCgi(argv);
	}

	return	Response(404).HttpResponse();
}

const std::string&	HttpRequestHandle::getMethod(const Request& req, const t_con& config) {
	if (this->validateUrlAllow(req.getUrl(), config, "GET")) {
		return this->getMethodRoute(req.getUrl(), req, config);
	}
	return Response(404).HttpResponse();
}

//Method POST



//Method DELETE

const std::string&	HttpRequestHandle::validateMethod(const Request& req,const t_con& config) {

	if (req.getMethod() == "GET") {
		return this->getMethod(req, config);
	}
	else if (req.getMethod() == "POST") {
		return "POST";
	}
	else if (req.getMethod() == "DELETE") {
		return "DELETE";
	}
	return Response(404).HttpResponse();
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


// std::string HttpRequestHandle::getMethod(const Request& req)
// {
// 	std::stringstream path;
// 	if (!this->_cgi_path.empty() || this->_cgi_path != "") {
// 		if (req.getPath() == "/" || req.getPath() == "/index.html")
// 			path << this->_cgi_path << "/index.html";
// 		else
// 			path << this->_cgi_path << req.getPath();
// 		return this->readFile(path);
// 	}
// 	Response err404(404, "not Found", "file not found");
// 	return err404.HttpResponse();
// }

// std::string HttpRequestHandle::getMethod(const Request& req)
// {
// 	int requestPort = getPortFromRequest(req);
// 	// Find the corresponding ServerConfig for the port
// 	std::string landingPage;
// 	for (std::vector<ServerConfig>::const_iterator it = _serverConfigs.begin(); it != _serverConfigs.end(); ++it)
// 	{
// 		if (it->port == requestPort)
// 		{

// 			landingPage = it->landingPagePath;
// 			break;
// 		}
// 	}

// 	if (landingPage.empty())
// 	{
// 		Response err404(404, "not Found", "file not found");
// 		return err404.HttpResponse();
// 	}

// 	std::stringstream path;
// 	if (!_cgi_path.empty() || _cgi_path != "")
// 	{
// 		if (req.getPath() == "/" || req.getPath() == "/index.html")
// 			path << _cgi_path << "/" << landingPage;
// 		else
// 			path << _cgi_path << req.getPath();
// 		std::cout << "-------path << _cgi_path << req.getPath()-----------" << std::endl;
// 		std::cout << path.str() << std::endl;
// 		std::cout << _cgi_path << std::endl;
// 		std::cout << req.getPath() << std::endl;
// 		std::cout << "-------------------" << std::endl;
// 		return this->readFile(path);
// 	}

// 	Response err404(404, "not Found", "file not found");
// 	return err404.HttpResponse();
// }


// std::string HttpRequestHandle::postMethod(const Request& req, Store *st)
// {
// 	Console::log(0, "hello");
// 	std::cout << "-------------------" << std::endl;
// 	std::string		content = req.getHeaderValue("Content-Type");
// 	size_t			pos = content.find("multipart/form-data;");
// 	std::cout << pos << std::endl;
// 	if (pos != std::string::npos && st->getMultiPart() == false) {
// 		st->setMultiPart(true);
// 	}
// 	Console::log(1, "HELLOOOOOOOOOOO POST\n");
// 	std::cout << "-------------------" << std::endl;
// 	// (void)st;
// 	Response err404(404, "not Found", "file not found");
// 	return err404.HttpResponse();
// }


//get path and delete file
// std::string HttpRequestHandle::deleteMethod(const Request& req)
// {
// 	(void)req;
// 	Response err404(404, "not Found", "file not found");
// 	return err404.HttpResponse();
// }

// std::string HttpRequestHandle::readFile(std::stringstream& path) {
// 	std::ifstream file(path.str(), std::ios::in | std::ios::binary);
// 	if (file.is_open())
// 	{
// 		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
// 		std::cout << content << std::endl;
// 		Console::modeMsg(1, content + "\n");
// 		file.close();
// 		Response res(200, "Ok", content);
// 		return res.HttpResponse();
// 	}
// 	Response err404(404, "not Found", "file not found");
// 	return err404.HttpResponse();
// }
