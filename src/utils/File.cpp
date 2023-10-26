/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 04:27:56 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/27 00:29:47 by psuanpro         ###   ########.fr       */
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
