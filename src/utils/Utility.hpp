/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:42:41 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/15 22:49:04 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <vector>

class Utility {
	private:
	public:
		Utility();
		~Utility();
		static std::vector<std::string>	splite(const std::string& str, const std::string& delim);
};

#endif
