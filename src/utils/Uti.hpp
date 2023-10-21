/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uti.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:42:41 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/20 02:45:26 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTI_HPP
# define UTI_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <vector>

class Uti {
	private:
	public:
		Uti();
		~Uti();
		static std::vector<std::string>	splite(const std::string& str, const std::string& delim);
		static std::vector<std::string>	spliteHeader(const std::string& str, const std::string& delim);
		static std::string				trim(const std::string& str, const std::string& delim);
};

#endif
