#include "Detail.hpp"


Detail::Detail() {
		this->_status = false;
		this->_detail = "";
		this->_serverConf = t_serverConf();
}

Detail::Detail(bool status, const std::string& detail, const t_serverConf& serverConf) {
		this->_status = status;
		this->_detail = detail;
		this->_serverConf = serverConf;
}

Detail::Detail(bool status) {
		this->_status = status;
		this->_detail = "";
		this->_serverConf = t_serverConf();
}

Detail::Detail(bool status, const std::string& detail) {
		this->_status = status;
		this->_detail = detail;
		this->_serverConf = t_serverConf();
}

Detail::Detail(bool status, const t_serverConf& serverConf) {
		this->_status = status;
		this->_detail = "";
		this->_serverConf = serverConf;
}

Detail::Detail(bool status, const std::vector<t_location>& locations){
	this->_status = status;
	this->_locations = locations;
}

Detail::Detail(bool status, const t_location& location) {
	this->_status = status;
	this->_location = location;
}

Detail::~Detail() {
}

bool		Detail::getStatus() const {
		return this->_status;
}

std::string	Detail::getDetail() const {
		return this->_detail;
}

void		Detail::setStatus(bool status) {
		this->_status = status;
}

void		Detail::setDetail(std::string detail) {
		this->_detail = detail;
}

void		Detail::setServerConf(t_serverConf serverConf) {
		this->_serverConf = serverConf;
}

t_detail	Detail::getDetailStruct() const {
		return (t_detail){this->_status, this->_detail, this->_serverConf, this->_locations, this->_location};
}



