/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandle.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:25:28 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/14 00:20:17 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandle.hpp"

FileHandle::FileHandle() {

}

FileHandle::FileHandle(const std::string& name, const std::string& boundary, const std::string& content):
	_name(name), _boundary(boundary), _content(content) {
}

FileHandle::~FileHandle() {

}

const std::string&	FileHandle::getName() const {
	return this->_name;
}

const std::string&	FileHandle::getBoundary() const {
	return this->_boundary;
}

const std::string&	FileHandle::getContent() const {
	return this->_content;
}

void	FileHandle::setName(const std::string& name) {
	this->_name = name;
	return ;
}

void	FileHandle::setBoundary(const std::string& boundary) {
	this->_boundary = boundary;
	return ;
}

void	FileHandle::setContentOrAdd(const std::string& content) {

	if (this->_content.empty())
		this->_content = content;
	else {
		std::stringstream	ss(this->_content);

		ss << content;
		this->_content.clear();
		this->_content = ss.str();
		return ;
	}
	return ;
}




