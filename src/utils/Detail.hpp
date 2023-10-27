#ifndef DETAIL_HPP
# define DETAIL_HPP

# include <iostream>
# include <string>

typedef struct  s_detail {
  bool        status;
  std::string detail;
}               t_detail;

class Detail {
    public:
        Detail();
        Detail(bool status, std::string detail);
        ~Detail();

        bool        getStatus() const;
        std::string getDetail() const;

        void        setStatus(bool status);
        void        setDetail(std::string detail);
        t_detail    getDetailStruct() const;
    private:
        bool        _status;
        std::string _detail;
};


#endif
