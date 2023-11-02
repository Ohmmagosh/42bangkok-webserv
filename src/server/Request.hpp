/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psuanpro <psuanpro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:17:34 by psuanpro          #+#    #+#             */
/*   Updated: 2023/11/02 18:55:38 by psuanpro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../stdlib.hpp"


class Request {
	private:
		std::string							_method;
		std::string							_url;
		std::string							_version;
		std::string							_body;
		std::string							_boundary;
		std::string							_content;
		std::string							_filename;
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
		std::string									getHeadersByValue(const std::string& key) ;
		const std::map<std::string, std::string>&	getHeaderC() const;
		const std::map<std::string, std::string>&	getQueryUrl() const;
		std::string									getQueryUrlByValue(const std::string& val);
		std::string									getContent() const;

		//helper
		bool										validateHeaderMethod(const std::string& line);
		void										validateHeadersCandNoC(const std::string& line);
		bool										validateBoundary();
		bool										validateMultiPart();
		bool										validateParams();
		bool										validateBoundaryBody() const;
		bool										validateBoundaryHeader() const;
		std::string									deleteBoundaryFromContent(const std::string& content);
		size_t										lenLine(const std::string& content);

		//setter
		void										setBoundaryFromContent(const std::string& content);
		void										setAddHeaderFromBody(const std::string& body);
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
		void										setAllContentMultipart(const std::string& content);
		void										setQueryUrl(const std::string& url);
};

std::ostream& operator<<(std::ostream& os, const Request& req);
#endif
