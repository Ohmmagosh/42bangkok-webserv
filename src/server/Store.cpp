/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Store.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:52:22 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/16 20:40:46 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Store.hpp"

Store::Store() {

}

Store::~Store() {

}

const FileHandle*	Store::FindFileVectorByName(const std::string& name) const {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			return &this->_file[i];
		}
	}
	return nullptr ;
}

void	Store::addFileVector(const FileHandle& file) {
	this->_file.push_back(file);
}

bool	Store::validateGotFileVectorByName(const std::string& name) const {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			return true ;
		}
	}
	return false ;
}

void	Store::delFileVectorByName(const std::string& name) {

	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			this->_file.erase(this->_file.begin() + i);
			return ;
		}
	}
	return ;
}

void	Store::updateContentFileVectorByName(const std::string& name, const std::string& content) {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			this->_file[i].setContentOrAdd(content);
			return ;
		}
	}
	return ;
}

bool	Store::createFileByName(const std::string& name)const {
	std::ofstream	file(name);

	if (!file) {
		return false;
	}

	file << this->_file[1].getContent();
	file.close();
	return true;
}

const std::string	Store::getFileContent(const std::string& path) const {
	std::ifstream	rfile(path);

	if (!rfile) {
		return NULL;
	}
	std::string	buff, content;
	while (std::getline(rfile, buff)) {
		content += buff;
	}
	rfile.close();
	return (content);
}

bool	Store::delFile(const std::string& name) {
	if (std::remove(name.c_str()) == 0) {
		return true;
	}
	return false;
}

bool	Store::validateContentBoundary() const {
	return true;
}
