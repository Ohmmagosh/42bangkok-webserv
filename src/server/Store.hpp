/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Store.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:52:26 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/18 02:39:19 by psuanpro         ###   ########.fr       */
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
		bool					_multipart;
	public:
		Store();
		~Store();
		const FileHandle*	FindFileVectorByName(const std::string& name) const;
		const FileHandle*	FildFileVectorByBoundary(const std::string& boundary) const;
		bool				validateGotFileVectorByName(const std::string& name) const;
		void				addFileVector(const FileHandle& file);
		void				delFileVectorByName(const std::string& name);
		void				updateContentFileVectorByName(const std::string& name, const std::string& content);
		void				updateContentFileVectorByBoundary(const std::string& boundary);
		bool				createFileByName(const std::string& name) const;
		const std::string	getFileContent(const std::string& path) const;
		bool				delFile(const std::string& name);
		bool				validateContentBoundary() const;
		void				setMultiPart(bool b);
		bool				getMultiPart() const;
};

#endif
