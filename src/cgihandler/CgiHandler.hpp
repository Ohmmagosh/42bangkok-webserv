/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:13 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/04 15:10:19 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../stdlib.hpp"

class CgiHandler {
	private:
		char	**_env;
	public:
		CgiHandler(/* args */);
		~CgiHandler();
		std::string	executeCgi();
	class PipeFail: public std::exception {
		const char* what() const throw () {
			return "Pipe : can not pipe";
		};
	};
};


#endif
