/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Store.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:52:26 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/14 00:35:50 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STORE_HPP
# define STORE_HPP

# include "../stdlib.hpp"
# include "FileHandle.hpp"

class Store
{
	private:
		std::vector<FileHandle>	_file;
	public:
		Store();
		~Store();
		const FileHandle*	getFileByName(const std::string& name) const;
		bool				searchFileName(const std::string& name) const;
		void				addFile(const FileHandle& file);
		void				delFileByName(const std::string& name);
		void				updateContentFileByName(const std::string& name, const std::string& content);
};

#endif
