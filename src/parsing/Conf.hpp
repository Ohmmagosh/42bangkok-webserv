/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaewsae <lkaewsae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 22:54:43 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/23 23:51:40 by lkaewsae         ###   ########.fr       */
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
		
		std::string	getServerValue(const std::string & key);
		std::string	getPagesValue(const std::string & key);
		std::string	getHttpValue(const std::string & key);
		std::string	getLocationValue(const std::string & key);
		//std::string	getLocationmapValue(const std::string & key);
		
		void	setServerValue(const std::string & key, const std::string & value);
		void	setPagesValue(const std::string & key, const std::string & value);
		void	setHttpValue(const std::string & key, const std::string & value);
		void	setLocationValue(const std::string & key, const std::string & value);
		//void	setLocationmapValue(const std::string & key, const std::string & value);
		
		void	readConfigFile(const std::string &configFile);
		std::map<std::string, std::string>::const_iterator findKey(const std::map<std::string, std::string> &map, const std::string& key);
	private:
		std::map<std::string, std::string>	_server;
		std::map<std::string, std::string>	_pages;
		std::map<std::string, std::string>	_http;
		std::map<std::string, std::string>	_location;
		std::map<std::string, std::string> _locationmap;
	class ValueNotFound:public std::exception {
		public:
			const char* what() const throw () {
				return "Value not found";
		};
	};
};

#endif
