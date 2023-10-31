/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uti.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:42:35 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/31 16:28:36 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uti.hpp"

Uti::Uti() {

}



Uti::~Uti() {

}


std::vector<std::string>	Uti::splite(const std::string& str, const std::string& delim) {
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

std::vector<std::string>	Uti::spliteHeader(const std::string& str, const std::string& delim) {
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

std::string	Uti::trim(const std::string& str, const std::string& delim) {
	std::string::size_type	start = str.find_first_not_of(delim);
	std::string::size_type	end = str.find_last_not_of(delim);
	if (start == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

int	Uti::strToInt(const std::string& str) {
	int	ret;
	std::stringstream	ss(str);

	ss >> ret;
	return ret;
}
