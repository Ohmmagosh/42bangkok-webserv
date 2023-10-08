/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 22:01:13 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/05 09:01:34 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../stdlib.hpp"
# include "../server/Request.hpp"

class CgiHandler {
	private:
		char	**_env;
	public:
		CgiHandler(/* args */);
		~CgiHandler();
		std::string	executeCgi();
		char** initEnv(const Request & req);
	class PipeFail: public std::exception {
		const char* what() const throw () {
			return "Pipe : can not pipe";
		};
	};
};


#endif
