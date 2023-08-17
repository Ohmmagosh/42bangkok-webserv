/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 22:59:20 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/18 00:47:21 by psuanpro         ###   ########.fr       */
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
		this->conf = rhs.conf;
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
	const std::map<std::string, std::string>::const_iterator it = findKey(this->conf, key);

	if (it == this->conf.end())
		throw ValueNotFound();
	return it->second;
}

void	Conf::setValue(const std::string & key, const std::string & value) {
	this->conf.insert(std::make_pair(key, value));
}
