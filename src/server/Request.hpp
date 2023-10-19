/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:17:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/10/19 07:34:04 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../stdlib.hpp"


// class Request
// {
// 	private:
// 		std::string							_method;
// 		std::string							_path;
// 		std::string							_protocol;
// 		std::vector<std::string>			_keys;
// 		std::string							_body;
// 		std::vector<std::string>			_req;
// 		std::map<std::string, std::string>	_params;
// 		std::map<std::string, std::string>	_headers;
// 	public:
// 		Request();
// 		Request(const std::string& raw_request);
// 		~Request();

// 		int											getHeaderSize() const;
// 		const std::string&							getMethod() const;
// 		const std::string&							getPath() const;
// 		const std::string&							getProtocol() const;
// 		std::string									getHeaderValue(const std::string& key) const;
// 		const std::string&							getBody() const;
// 		const std::vector<std::string>&				getKeysHeader() const;
// 		const std::map<std::string, std::string>&	getMapHeader() const;
// 		int											getReqVectorSize()const;
// 		const std::vector<std::string>&				getVectorReq() const;
// 		void										setHeader(const std::string& header);
// 		void										setRawReqToVector(const std::string& raw_request);
// 		void										setHeaderAndBody(const std::vector<std::string>& req);
// 		bool										isMethod(const std::string& req_header);
// 		void										setBody(const std::string& body);
// 		void										setParams(const std::string& params);
// 		const std::map<std::string, std::string>&	getQueryParams() const;



// 	class HeaderNotFound: public std::exception {
// 		private:
// 			std::string	_message;
// 		public:
// 			HeaderNotFound(const std::string& key) {
// 				_message = key;
// 				_message += ": Not found";
// 			}
// 			~HeaderNotFound() throw() {};
// 			const char* what() const throw() {
// 				return _message.c_str();
// 			};
// 	};

// };
// std::ostream& operator<<(std::ostream& os, const  Request& req);
// std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec);

class Request {
	private:
		std::string							_method;
		std::string							_url;
		std::string							_version;
		std::string							_body;
		std::string							_boundary;
		std::vector<std::string>			_headers_no_c;
		std::map<std::string, std::string>	_headers;
		std::map<std::string, std::string>	_query_url;

	public:
		Request();
		Request(const std::string& raw_req);
		~Request();

		//getter
		std::string									getMethod() const;
		std::string									getBody() const;
		std::string									getUrl() const;
		std::string									getVersion() const;
		std::string									getBoundary() const;
		const std::vector<std::string>&				getHeaderNoC() const;
		std::string									getHeadersByValue(const std::string& key);
		const std::map<std::string, std::string>&	getHeaderC() const;
		const std::map<std::string, std::string>&	getQueryUrl() const;
		std::string							getQueryUrlByValue(const std::string& val);

		//helper
		bool										validateHeaderMethod(const std::string& line);
		void										validateHeadersCandNoC(const std::string& line);
		bool										validateBoundary();
		bool										validateMultiPart();
		bool										validateParams();
		bool										validateBoundaryBody() const;
		bool										validateBoundaryHeader() const;

		//setter
		void										setBoundaryFromContent(const std::string& content);
		void										setBoundary(const std::string& boundary);
		void										setHeaderMethod(const std::string& line);
		void										setMethod(const std::string& method);
		void										setBody(const std::string& body);
		void										setUrl(const std::string& method);
		void										setVersion(const std::string& method);
		void										setHeaderC(const std::string& line);
		void										setHeaderNoC(const std::string& line);
		void										setHeaderAndBody(const std::string& raw_req);
		void										setAllHeader(const std::string& header);
		void										setAllBody(const std::string& body);
		void										setQueryUrl();


};

std::ostream& operator<<(std::ostream& os, const Request& req);
#endif
