/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 22:54:43 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/18 00:46:30 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_HPP
# define CONF_HPP

# include "../stdlib.hpp"


class Conf
{
	public:
		Conf();
		Conf(Conf const & rhs);
		~Conf();

		Conf &	operator=(Conf const & rhs);
		std::string getValue(const std::string & key);
		void	setValue(const std::string & key,const std::string & value);
		std::map<std::string, std::string>::const_iterator findKey(const std::map<std::string, std::string> &map, const std::string& key);
	private:
		std::map<std::string, std::string>	conf;
	class ValueNotFound:public std::exception {
		public:
			const char* what() const throw () {
				return "Value not found";
		};
	};
};

#endif
