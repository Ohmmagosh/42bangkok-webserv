/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Store.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:52:22 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/11 22:53:22 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Store.hpp"

Store::Store() {

}

Store::~Store() {

}

const std::string&	Store::getBoundary() const {
	return this->_boundary;
}

void	Store::setBoundary(const std::string& bound) {
	this->_boundary = bound;
}
