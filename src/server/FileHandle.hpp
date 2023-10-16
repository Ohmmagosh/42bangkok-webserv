/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandle.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:26:06 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/14 00:23:32 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEHANDLE_HPP
# define FILEHANDLE_HPP

# include "../stdlib.hpp"

class FileHandle {
	private:
		std::string	_name;
		std::string	_boundary;
		std::string	_content;
	public:
		FileHandle();
		FileHandle(const std::string& name, const std::string& boundary,const std::string& content);
		~FileHandle();

		void	setName(const std::string& name);
		void	setBoundary(const std::string& boundary);
		void	setContentOrAdd(const std::string& content);
		const std::string&	getName() const;
		const std::string&	getBoundary() const;
		const std::string&	getContent() const;
};

#endif
