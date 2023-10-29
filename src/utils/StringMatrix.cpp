/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringMatrix.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 02:46:42 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/29 14:46:09 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StringMatrix.hpp"

StringMatrix::StringMatrix(): _str(nullptr), _size(0)
{
}

StringMatrix::StringMatrix(const std::vector<std::string>& str) {
	this->_size = str.size();
	this->_str = new char*[this->_size + 1];

	size_t	i = 0;
	while(i < this->_size) {
		this->_str[i] = strdup(str[i].c_str());
		i++;
	}
	this->_str[this->_size] = nullptr;
}

StringMatrix::StringMatrix(const std::vector<std::string>& exe, const std::vector<std::string>& file) {
	this->_size = (exe.size() + file.size());
	this->_str = new char*[this->_size + 1];

	size_t i = 0;
	for (; i < exe.size();i++) {
		this->_str[i] = strdup(exe[i].c_str());
	}
	for (size_t j = 0; j < file.size(); j++,i++) {
		this->_str[i] = strdup(file[j].c_str());
	}
	this->_str[this->_size] = nullptr;
}

StringMatrix::StringMatrix(const std::map<std::string, std::string>& env) {
	this->_size = (env.size());
	this->_str = new char*[this->_size + 1];

	std::map<std::string, std::string>::const_iterator	it = env.begin();
	size_t i = 0;
	for (;it != env.end(); i++,it++) {
		std::stringstream	ss;
		ss << it->first << "=" << it->second;
		this->_str[i] = strdup(ss.str().c_str());
	}
	this->_str[this->_size] = nullptr;
}

StringMatrix::~StringMatrix()
{
	for (size_t i = 0; i < this->_size;i++) {
		delete[] this->_str[i];
	}
	delete[] this->_str;
}

const char*	StringMatrix::operator[](size_t index) const{
	if (index > this->_size)
		return this->_str[this->_size];
	return this->_str[index];
}

char**	StringMatrix::getStr() const{
	return this->_str;
}

size_t	StringMatrix::size() const {
	return	this->_size;
}

std::ostream&	operator<<(std::ostream& os, const StringMatrix& s) {
	for (size_t i = 0; i < s.size(); i++) {
		os << s[i] << std::endl;
	}
	return os;
}
