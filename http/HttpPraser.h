#include <vector>

#include "HttpRequest.h"

class HttpPraser {
 public:
  enum CHECK_STATUS {
    PRASE_LINE,
    PRASE_HEAD,
    PRASE_BODY,
  };
  enum LINE_STATUS {
    LINE_OK,
    LINE_BAD,
    LINE_OPEN,
  };
  enum HTTP_CODE {
    NO_REQUEST,
    GET_REQUEST,
    BAD_REQUEST,
  };

  HttpPraser()
      : data_index_(0),
        read_index_(1),
        check_index_(0),
        start_line_(0),
        check_status_(PRASE_LINE){
            // buf.reserve(1024);
        };
  HttpPraser(char* buffer, int len)
      : buf(buffer),
        data_index_(0),
        read_index_(1),
        check_index_(0),
        start_line_(0),
        check_status_(PRASE_LINE){
            read_index_ += len;
        };
  void prase_line();
  void prase_questline();
  void prase_header();
  void prase_body(size_t len);
  HTTP_CODE prase();
  void append(const char* data, size_t len);
  HttpRequest getrequest() { return request_; }
 
  char* getbuf() { return buf; }
  char* begin() { return buf + read_index_ - 1; }

 private:
  CHECK_STATUS check_status_;
  LINE_STATUS line_status_;
  HTTP_CODE http_code_;
  HttpRequest request_;

  int data_index_;
  int read_index_;
  int check_index_;
  int start_line_;
  char* buf;
  // std::vector<char> buf;
};