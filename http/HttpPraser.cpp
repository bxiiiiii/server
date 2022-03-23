#include "HttpPraser.h"

void HttpPraser::prase_line() {
  char temp;
  for (; check_index_ < read_index_; ++check_index_) {
    temp = buf[check_index_];
    if (temp == '\r') {
      if ((check_index_ + 1) == read_index_) {
        line_status_ = LINE_OPEN;
        goto over;
      } else if (buf[check_index_ + 1] == '\n') {
        line_status_ = LINE_OK;
        buf[check_index_++] = '\0';
        buf[check_index_++] = '\0';
        goto over;
      }
      line_status_ = LINE_BAD;
      return;
    } else if (temp == '\n') {
      if ((check_index_ > 1) && buf[check_index_ - 1] == '\r') {
        buf[check_index_ - 1] = '\0';
        buf[check_index_++] = '\0';
        line_status_ = LINE_OK;
        goto over;
      }
      line_status_ = LINE_BAD;
      goto over;
    }
  }
  line_status_ = LINE_BAD;
over:
}

void HttpPraser::prase_questline() {
  for (int i = start_line_; i < read_index_;) {
    if (strncmp(buf + i, "GET", strlen("GET")) == 0) {
      request_.setMethod("GET", strlen("GET"));
      i += strlen("Get");
    } else if (strncmp(buf + i, "Head", strlen("Head")) == 0) {
      request_.setMethod("Head", strlen("Head"));
      i += strlen("Head");
    } else if (strncmp(buf + i, "POST", strlen("POST")) == 0) {
      request_.setMethod("Post", strlen("Post"));
      i += strlen("Post");
    } else {
      line_status_ = LINE_BAD;
      return;
    }
    if (buf[i++] != ' ') {
      return;
    }

    request_.setPath(string(buf + begin, i - begin - 1), i - begin - 1);

    if (strncmp(buf + i, "HTTP/1.0", strlen("HTTP/1.0")) == 0 ||
        strncmp(buf + i, "HTTP/1.1", strlen("HTTP/1.1")) == 0) {
              request_.setVersion(string(buf + i, strlen("HTTP/1.0")), strlen("HTTP/1.0"));
    }
  }
}

HttpPraser::HTTP_CODE HttpPraser::prase() {
  do {
    prase_line();
    switch (check_status_) {
      case PRASE_LINE:
        prase_questline();
        break;
      case PRASE_HEAD:
        prase_header();
        break;
      case PRASE_BODY:
        prase_body();
        break;
      default:
        break;
    }

  } while (line_status_ == LINE_OK);
}