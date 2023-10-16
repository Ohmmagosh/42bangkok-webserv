/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Console.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 02:21:29 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/16 23:59:27 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Console.hpp"

Console::Console() {
	std::string		d(YELB);
	std::string		i(BLUB);
	std::string		e(REDB);
	this->_mode[0] = (t_mode){"[DEBUG]", d};
	this->_mode[1] = (t_mode){"[INFO]", i};
	this->_mode[2] = (t_mode){"[ERROR]", e};
}

Console::~Console() {

}

void	Console::cmMsg(int mode, const std::string& content, const std::string& header, const std::string& end){
	Console				c;
	t_mode				mc = c.getModeAndColor(mode);
	std::cout << mc.color << "--------------------" << header << "---------------------" << RES << std::endl;
	c.initMsg(mc, content);
	std::cout << mc.color << "--------------------" << end << "---------------------" << RES << std::endl;
	return ;
}

void	Console::modeMsg(int mode, const std::string& msg) {
	Console				c;
	t_mode				mc = c.getModeAndColor(mode);

	c.initMsg(mc, msg);
	return ;
}

void	Console::initMsg(t_mode	mc, const std::string& msg) {
	std::stringstream	ss;
	ss << mc.color << "--------------------" << mc.mode << "---------------------" << RES << std::endl;
	ss << msg << std::endl;
	ss << mc.color << "----------------------END-----------------------" << RES << std::endl;
	if (mc.mode == "[ERROR]")
		std::cerr << ss.str();
	std::cout << ss.str();
	return ;
}

const std::string& Console::getMode(int mode) {
	return this->_mode[mode].mode;
}

const std::string&	Console::getModeColor(int mode) {
	return this->_mode[mode].color;
}

t_mode		Console::getModeAndColor(int mode) {
	return this->_mode[mode];
}

void	Console::log(int mode, const std::string& msg) {
	Console				c;
	std::stringstream	ss;
	t_mode				mc = c.getModeAndColor(mode);
	ss << mc.color << mc.mode << RES << " " << msg << std::endl;

	if (mode == 2)
		std::cerr << ss.str();
	std::cout << ss.str();
	return ;
}
