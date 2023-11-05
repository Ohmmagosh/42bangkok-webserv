/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 04:27:56 by psuanpro          #+#    #+#             */
/*   Updated: 2023/11/03 15:28:29 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"




File::File(){

}

File::File(const std::string& name): _name(name){
	this->readFile();
}

File::~File() {

}

void	File::readFile() {
	std::ifstream	f(this->_name);

	if(!f.is_open()) {
		return ;
	}
	std::string	buff;
	while(std::getline(f, buff)) {
		this->_content += buff;
		this->_content += '\n';
	}
	return ;
}

std::string	File::getContent() const {
	return this->_content;
}

int	File::deleteFile(const std::string& filename) const{
	if (access(filename.c_str(), F_OK ) != 0)
		return -1;
	if (std::remove(filename.c_str()) !=  0)
		return -2;
	return 0;
}
