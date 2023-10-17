/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:17:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/18 03:26:43 by psuanpro         ###   ########.fr       */
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
		std::vector<std::string>			_keys;
		std::string							_body;
		std::vector<std::string>			_req;
		std::map<std::string, std::string>	_params;
		std::map<std::string, std::string>	_headers;
	public:
		Request();
		Request(const std::string& raw_request);
		~Request();

		int											getHeaderSize() const;
		const std::string&							getMethod() const;
		const std::string&							getPath() const;
		const std::string&							getProtocol() const;
		std::string									getHeaderValue(const std::string& key) const;
		const std::string&							getBody() const;
		const std::vector<std::string>&				getKeysHeader() const;
		const std::map<std::string, std::string>&	getMapHeader() const;
		int											getReqVectorSize()const;
		const std::vector<std::string>&				getVectorReq() const;
		void										setHeader(const std::string& header);
		void										setRawReqToVector(const std::string& raw_request);
		void										setHeaderAndBody(const std::vector<std::string>& req);
		bool										isMethod(const std::string& req_header);
		void										setBody(const std::string& body);
		void										setParams(const std::string& params);
		const std::map<std::string, std::string>&	getQueryParams() const;



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
std::ostream& operator<<(std::ostream& os, const  Request& req);
std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec);
#endif
