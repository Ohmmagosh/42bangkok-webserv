/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:42:35 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/15 22:52:45 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility.hpp"

Utility::Utility() {

}



Utility::~Utility() {

}


std::vector<std::string>	Utility::splite(const std::string& str, const std::string& delim) {
	std::vector<std::string>	ret;
	size_t						start = 0;
	size_t						end = str.find(delim);

	while (end != std::string::npos) {
		ret.push_back(str.substr(start , end - start));
		start = end + delim.length();
		end = str.find(delim, start);
	}
	ret.push_back(str.substr(start, end ));
	return ret;
}
