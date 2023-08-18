/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/18 18:03:46 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

Parsing::Parsing()
{

}

Parsing::Parsing(const std::string & file_path) {
	this->_file.open(file_path.c_str(), std::ifstream::in);
}

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
	{

	}
	return (*this);
}

void	Parsing::readFile(void) {
	if (!this->_file.is_open())
		throw CanNotOpenFile();

}
