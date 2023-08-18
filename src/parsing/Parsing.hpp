/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 20:56:33 by psuanpro          #+#    #+#             */
/*   Updated: 2023/08/18 18:00:48 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include "../stdlib.hpp"

class Parsing
{
	private:
		std::fstream	_file;
	public:
		Parsing();
		Parsing(const std::string & file_path);
		Parsing(Parsing const & src);
		~Parsing();

		Parsing &	operator=(Parsing const & rhs);
		void	readFile(void);
		class CanNotOpenFile: public std::exception {
			public:
				const char* what() const throw () {
					return "Can not open file";
				};
		};

};

#endif
