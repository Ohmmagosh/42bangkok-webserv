/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchiewli <rchiewli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:17:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/10 22:56:08 by rchiewli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../stdlib.hpp"

class Request
{
	private:
		std::string							_method;
		std::string							_path;
		std::string							_protocol;
		std::map<std::string, std::string>	_headers;
	public:
		Request();
		Request(const std::string& raw_request);
		~Request();
		const std::string& getMethod() const;
		const std::string& getPath() const;
		const std::string& getProtocol() const;
		const std::string& getHeader(const std::string& key) const;

	class HeaderNotFound: public std::exception {
		private:
			std::string	_message;
		public:
			HeaderNotFound(const std::string& key) {
				_message = key;
				_message += ": Not found";
			}
			~HeaderNotFound() throw() {};
			const char* what() const throw() {
				return _message.c_str();
			};
	};

};

#endif
