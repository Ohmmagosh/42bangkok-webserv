/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Store.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:52:26 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/11 22:53:20 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STORE_HPP
# define STORE_HPP

# include "../stdlib.hpp"

class Store
{
	private:
		std::string	_boundary;
	public:
		Store();
		~Store();
		const std::string&	getBoundary() const;
		void			setBoundary(const std::string& bound);
};

#endif
