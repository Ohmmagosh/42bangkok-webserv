/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:42:35 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/19 05:00:08 by psuanpro         ###   ########.fr       */
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

std::vector<std::string>	Utility::spliteHeader(const std::string& str, const std::string& delim) {
	std::vector<std::string>	ret;
	size_t						start = 0;
	size_t						end = str.find(delim);
	int							i = 0;

	while (end != std::string::npos && i < 1) {
		ret.push_back(str.substr(start , end - start));
		start = end + delim.length();
		end = str.find(delim, start);
		i++;
	}
	ret.push_back(str.substr(start, str.length() - 1 ));
	return ret;
}

std::string	Utility::trim(const std::string& str, const std::string& delim) {
	std::string::size_type	start = str.find_first_not_of(delim);
	std::string::size_type	end = str.find_last_not_of(delim);
	if (start == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}
