/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaewsae <lkaewsae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 22:59:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/23 01:38:59 by lkaewsae         ###   ########.fr       */
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
		this->_conf = rhs._conf;
	}
	return (*this);
}

Conf::~Conf() {

}

std::map<std::string, std::string>::const_iterator	Conf::findKey(const std::map<std::string, std::string> &imap, const std::string& key) {
	std::map<std::string, std::string>::const_iterator	ret = imap.find(key);
	return ret;
}

std::string	Conf::getValue(const std::string & key){
	const std::map<std::string, std::string>::const_iterator it = findKey(this->_conf, key);

	if (it == this->_conf.end())
		throw ValueNotFound();
	return it->second;
}

void	Conf::setValue(const std::string & key, const std::string & value) {
	this->_conf.insert(std::make_pair(key, value));
}

bool Conf::IsNotSpace(int c)
{
	return (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\f' && c != '\v');
}

void Conf::readConfigFile(const std::string &configFile)
{
	std::istringstream iss(configFile);
	std::string currenttext;
	std::string line;

	while (std::getline(iss, line))
	{
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), IsNotSpace));
		line.erase(std::find_if(line.rbegin(), line.rend(), IsNotSpace).base(), line.end());
		if (line.empty() || line[0] == '#')
			continue;
	}
	if (line[0] == '[' && line[line.size() - 1] == ']')
	{
		currenttext = line.substr(1, line.size() - 2);
	}
	else
	{
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
				this->_conf[currenttext + "." + key] = value;
			}
		}
	}
}