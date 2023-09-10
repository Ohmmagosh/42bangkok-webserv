/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:33 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/10 22:49:26 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include "../stdlib.hpp"
# include "Conf.hpp"
# include <cstring>

class Parsing
{
	private:
		std::fstream	_file;
		std::string		_file_path;
		std::string		_text_file;
		Conf			*_config;
		// int				_config_size;
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
				CanNotOpenFile(const std::string & file_name): _message(file_name) {
					_message = file_name;
					_message += " : Not found";
				}
				virtual const char* what(void) const throw () {
					return "Error";
				};
				virtual ~CanNotOpenFile() throw () {}
		};
		class UnmatchedBracketException: public std::exception {
			public:
				virtual const char* what(void) const throw () {
					return "Unmatched Bracket []";
				};
		};
};

#endif
