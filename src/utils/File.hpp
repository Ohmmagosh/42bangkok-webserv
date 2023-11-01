/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 04:28:01 by psuanpro          #+#    #+#             */
/*   Updated: 2023/11/02 03:20:40 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	FILE_HPP
# define FILE_HPP

# include <string>
# include <fstream>
# include <iostream>
# include <unistd.h>
# include <cstdio>


class	File {
	private:
		std::string	_name;
		std::string	_content;
		void				readFile();
	public:
		File();
		File(const std::string& name);
		~File();

		std::string			getContent() const;
		int					deleteFile(const std::string& filename) const;

};

#endif
