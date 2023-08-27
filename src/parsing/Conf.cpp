/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaewsae <lkaewsae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 22:59:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/24 00:33:37 by lkaewsae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Conf.hpp"

Conf::Conf() {

}

Conf::Conf(Conf const & rhs) {
	*this = rhs;
}

Conf & Conf::operator=(Conf const & rhs) {
	if (this != &rhs) {
		this->_server = rhs._server;
		this->_pages = rhs._pages;
		this->_http = rhs._http;
	}
	return (*this);
}

Conf::~Conf() {

}

std::map<std::string, std::string>::const_iterator	Conf::findKey(const std::map<std::string, std::string> &imap, const std::string& key) {
	std::map<std::string, std::string>::const_iterator	ret = imap.find(key);
	return ret;
}

std::string	Conf::getServerValue(const std::string & key){
	const std::map<std::string, std::string>::const_iterator it = findKey(this->_server, key);

	if (it == this->_server.end())
		throw ValueNotFound();
	return it->second;
}

std::string	Conf::getPagesValue(const std::string & key){
	const std::map<std::string, std::string>::const_iterator it = findKey(this->_pages, key);

	if (it == this->_pages.end())
		throw ValueNotFound();
	return it->second;
}

std::string	Conf::getHttpValue(const std::string & key){
	const std::map<std::string, std::string>::const_iterator it = findKey(this->_http, key);

	if (it == this->_http.end())
		throw ValueNotFound();
	return it->second;
}

std::string	Conf::getLocationValue(const std::string & key){
	const std::map<std::string, std::string>::const_iterator it = findKey(this->_location, key);

	if (it == this->_location.end())
		throw ValueNotFound();
	return it->second;
}

// std::string	Conf::getLocationmapValue(const std::string & key){
// 	const std::map<std::string, std::string>::const_iterator it = findKey(this->_locationmap, key);

// 	if (it == this->_locationmap.end())
// 		throw ValueNotFound();
// 	return it->second;
// }

void	Conf::setServerValue(const std::string & key, const std::string & value) {
	this->_server.insert(std::make_pair(key, value));
}

void	Conf::setPagesValue(const std::string & key, const std::string & value) {
	this->_pages.insert(std::make_pair(key, value));
}

void	Conf::setHttpValue(const std::string & key, const std::string & value) {
	this->_http.insert(std::make_pair(key, value));
}

void	Conf::setLocationValue(const std::string & key, const std::string & value) {
	this->_location.insert(std::make_pair(key, value));
}

// void	Conf::setLocationmapValue(const std::string & key, const std::string & value) {
// 	this->_locationmap.insert(std::make_pair(key, value));
// }

static bool IsNotSpace(int c)
{
	return (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\f' && c != '\v');
}

void Conf::readConfigFile(const std::string &configFile)
{
	std::istringstream iss(configFile);
	std::string currenttext;
	std::string line;
	std::string route;
	std::string root;

	while (std::getline(iss, line))
	{
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), IsNotSpace));
		line.erase(std::find_if(line.rbegin(), line.rend(), IsNotSpace).base(), line.end());
		//std::cout << "line : " << line << std::endl;
		if (line.empty() || line[0] == '#')
			continue;
		if (line[0] == '[' && line[line.size() - 1] == ']')
		{
			currenttext = line.substr(1, line.size() - 2);
		}
		std::size_t found = line.find("location");
		if (found!=std::string::npos)
		{
			currenttext = "location";
		}
		else
		{
			// std::cout << "line : " << line << std::endl;
			std::istringstream linesplit(line);
			std::string key, value;
			if (std::getline(linesplit, key, '='))
			{
				std::getline(linesplit, value);
				key.erase(key.begin(), std::find_if(key.begin(), key.end(), IsNotSpace));
				key.erase(std::find_if(key.rbegin(), key.rend(), IsNotSpace).base(), value.end());
				// Store the key-value pair in the map
				if (!currenttext.empty())
				{
					// std::cout << "currenttext : " << currenttext << std::endl;
					if (currenttext == "server")
						this->_server[key] = value;
					else if (currenttext == "pages")
						this->_pages[key] = value;
					else if (currenttext == "http")
						this->_http[key] = value;
					else  if (currenttext == "location")
					{
						std::istringstream linelocation(key);
						std::string locationKey, locationValue;
						std::getline(linelocation, locationKey, ' ');
						std::getline(linelocation, locationValue);
						locationKey.erase(locationKey.begin(), std::find_if(locationKey.begin(), locationKey.end(), IsNotSpace));
						locationKey.erase(std::find_if(locationKey.rbegin(), locationKey.rend(), IsNotSpace).base(), value.end());
						if (locationKey == "route")
							this->_location[locationKey] = locationValue;
						else if (locationKey == "root")
							this->_location[locationKey] = locationValue;
					}
				}
			}
		}
	}
}