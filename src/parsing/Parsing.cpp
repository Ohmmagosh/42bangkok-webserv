/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <Marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/07/17 21:06:16 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

Parsing::Parsing()
{

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
        //do something
    }
    return (*this);
}
