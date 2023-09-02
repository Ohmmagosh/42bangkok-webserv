/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:17:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/09/02 18:56:59 by psuanpro         ###   ########.fr       */
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
		std::map<std::string, std::string>	_query_string;
		// REQUEST_METHOD: The HTTP request method (e.g., GET, POST).
		// QUERY_STRING: The query string (the part of the URL after the ?).
		// CONTENT_TYPE: The media type of the data for POST requests.
		// CONTENT_LENGTH: The length of the data for POST requests.
		// HTTP_USER_AGENT: Information about the user agent (browser) making the request.
		// HTTP_REFERER: The URL of the page that linked to the resource being requested.
		// PATH_INFO: Additional path information given after the script name in the URL.
		// REMOTE_ADDR: The IP address of the client making the request.
		// SERVER_NAME: The name of the server on which the script resides.
		// SERVER_PORT: The port on the server where the request was received.
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
