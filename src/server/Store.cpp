/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Store.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:52:22 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/16 17:41:21 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Store.hpp"

Store::Store() {

}

Store::~Store() {

}

const FileHandle*	Store::getFileByName(const std::string& name) const {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			return &this->_file[i];
		}
	}
	return nullptr ;
}

void	Store::addFile(const FileHandle& file) {
	this->_file.push_back(file);
}

bool	Store::searchFileName(const std::string& name) const {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			return true ;
		}
	}
	return false ;
}

void	Store::delFileByName(const std::string& name) {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			this->_file.erase(this->_file.begin() + i);
			return ;
		}
	}
	return ;
}

void	Store::updateContentFileByName(const std::string& name, const std::string& content) {
	for (size_t i = 0; i < this->_file.size(); i++) {
		if (this->_file[i].getName() == name) {
			this->_file[i].setContentOrAdd(content);
			return ;
		}
	}
	return ;
}

