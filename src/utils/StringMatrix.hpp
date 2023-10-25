/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringMatrix.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 02:46:38 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/26 04:39:41 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGMATRIX_HPP
# define STRINGMATRIX_HPP

# include <ostream>
# include <vector>
# include <string>
# include <sstream>

class StringMatrix {
	private:
		char	**_str;
		size_t	_size;
	public:
		StringMatrix();
		~StringMatrix();
		StringMatrix(const std::vector<std::string>& str);
		StringMatrix(const std::vector<std::string>& exe, const std::vector<std::string>& file);
		char**	getStr() const;
		const char*	operator[](size_t index) const;
		size_t	size() const;
};

std::ostream&	operator<<(std::ostream& os, const StringMatrix& str);

#endif
