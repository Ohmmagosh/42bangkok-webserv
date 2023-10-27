#include "Detail.hpp"

Detail::Detail() {
    this->_status = false;
    this->_detail = "";
}

Detail::Detail(bool status, std::string detail) {
    this->_status = status;
    this->_detail = detail;
}

Detail::~Detail() {
}

bool        Detail::getStatus() const {
    return this->_status;
}

std::string Detail::getDetail() const {
    return this->_detail;
}

void        Detail::setStatus(bool status) {
    this->_status = status;
}

void        Detail::setDetail(std::string detail) {
    this->_detail = detail;
}

t_detail    Detail::getDetailStruct() const {
    return (t_detail){this->_status, this->_detail};
}
