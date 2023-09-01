/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandle.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:12:46 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/29 16:02:27 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestHandle.hpp"

HttpRequestHandle::HttpRequestHandle() {

}

HttpRequestHandle::HttpRequestHandle(const HttpRequestHandle & rhs) {
    (void)rhs;
}

HttpRequestHandle::~HttpRequestHandle() {

}

HttpRequestHandle& HttpRequestHandle::operator=(const HttpRequestHandle & rhs) {
    // if (this != &rhs) {  // Check for self-assignment
    //     this->someMemberVariable = rhs.someMemberVariable;
    //     // Copy other member variables
    // }
    (void)rhs;
    return *this;
}
