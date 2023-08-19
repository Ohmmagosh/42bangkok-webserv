/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/19 07:21:53 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

Parsing::Parsing()
{

}

Parsing::Parsing(const std::string & file_path): _file_path(file_path) {}

Parsing::Parsing(Parsing const & src)
{
	*this = src;
}

Parsing::~Parsing()
{

}

Parsing &	Parsing::operator=(Parsing const & rhs)
{
	if (this != &rhs)
		this->_file_path = rhs._file_path;
	return (*this);
}

void	Parsing::readFile(void) {
	this->_file.open(this->_file_path, std::fstream::in);
	if (!this->_file.is_open())
		throw CanNotOpenFile(this->_file_path);
	return ;
}

int		Parsing::countBracket(const std::string & text) {
	int count = 0;
	bool isOpenBracketFound = false;
	for (std::size_t i = 0; i < text.size(); ++i) {
		char c = text[i];
		if (c == '[') {
			isOpenBracketFound = true;
		} else if (c == ']' && isOpenBracketFound) {
			isOpenBracketFound = false;
			count++;
		}
	}
	if (isOpenBracketFound) {
		throw UnmatchedBracketException();
	}
	return count;
}
