/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:33 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/19 08:14:17 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include "../stdlib.hpp"
# include "Conf.hpp"

class Parsing
{
	private:
		std::fstream	_file;
		std::string		_file_path;
		std::string		_text_file;
		Conf			*_config;
		int				_config_size;
	public:
		Parsing();
		Parsing(const std::string & file_path);
		Parsing(Parsing const & src);
		~Parsing();

		Parsing &	operator=(Parsing const & rhs);
		int			countBracket(const std::string & text);
		void		readFile(void);

		class CanNotOpenFile: public std::exception {
			private:
				std::string	_message;
			public:
				CanNotOpenFile(const std::string & file_name) {
					_message = file_name;
					_message += " : Not found";
				}
				const char* what() const throw () {
					return _message.c_str();
				};
		};
		class UnmatchedBracketException: public std::exception {
			public:
				const char* what() const throw () {
					return "Unmatched Bracket []";
				};
		};
};

#endif
