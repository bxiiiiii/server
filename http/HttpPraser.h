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
        read_index_(sizeof(buf)),
        check_index_(0),
        start_line_(0),
        check_status_(PRASE_LINE){

        };

  void prase_line();
  void prase_questline();
  void prase_header();
  void prase_body(size_t len);
  HTTP_CODE prase();
  void append(const char* buf, size_t len);

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
};