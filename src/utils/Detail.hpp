#ifndef DETAIL_HPP
# define DETAIL_HPP

# include <iostream>
# include <string>
# include "../struct.hpp"

class Detail {
		public:
				Detail();
				Detail(bool status, const std::string& detail, const t_serverConf& serverConf);
				Detail::Detail(bool status);
				Detail::Detail(bool status, const std::string& detail);
				Detail::Detail(bool status, const t_serverConf& serverConf);
				~Detail();

				bool				getStatus() const;
				std::string	getDetail() const;
				t_detail		getDetailStruct() const;

				void				setStatus(bool status);
				void				setDetail(std::string detail);
				void				setServerConf(t_serverConf serverConf);
		private:
				bool					_status;
				std::string		_detail;
				t_serverConf	_serverConf;
};


#endif
