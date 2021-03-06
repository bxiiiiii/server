#include "HttpPraser.h"

#include <string>
void HttpPraser::prase_line() {
  LOG_DEBUG << check_index_ << " " << read_index_;
  char temp;
  start_line_ = check_index_;
  for (; check_index_ < read_index_; ++check_index_) {
    temp = buf[check_index_];
    LOG_DEBUG << check_index_ << " " << temp;
    if (temp == '\r') {
      if ((check_index_ + 1) == read_index_) {
        line_status_ = LINE_OPEN;
        return;
      } else if (buf[check_index_ + 1] == '\n') {
        line_status_ = LINE_OK;
        // buf[check_index_++] = '\0';
        // buf[check_index_++] = '\0';
        check_index_ += 2;
        LOG_DEBUG << line_status_;
        return;
      }
      line_status_ = LINE_BAD;
      return;
    } else if (temp == '\n') {
      if ((check_index_ > 1) && buf[check_index_ - 1] == '\r') {
        // buf[check_index_ - 1] = '\0';
        // buf[check_index_++] = '\0';
        check_index_++;
        line_status_ = LINE_OK;
        return;
      }
      line_status_ = LINE_BAD;
      return;
    }
  }
  line_status_ = LINE_BAD;
}

void HttpPraser::prase_questline() {
  LOG_DEBUG << "";
  prase_line();
  if (line_status_ != LINE_OK) {
    return;
  }
  LOG_DEBUG << line_status_;
  int i = start_line_;
  while (i <= check_index_) {
    if (strncmp(getbuf() + i, "GET", strlen("GET")) == 0) {
      request_.setMethod("GET", strlen("GET"));
      i += strlen("Get");
    } else if (strncmp(getbuf() + i, "Head", strlen("Head")) == 0) {
      request_.setMethod("Head", strlen("Head"));
      i += strlen("Head");
    } else if (strncmp(getbuf() + i, "POST", strlen("POST")) == 0) {
      request_.setMethod("Post", strlen("Post"));
      i += strlen("Post");
    } else {
      line_status_ = LINE_BAD;
      return;
    }

    if (buf[i++] != ' ') {
      line_status_ = LINE_BAD;
      return;
    }

    int url_start = i;
    while (buf[i] != ' ') {
      i++;
    }
    request_.setPath(string(getbuf() + url_start, i - url_start - 1).data(),
                     i - url_start - 1);

    if (strncmp(getbuf() + i, "HTTP/1.0", strlen("HTTP/1.0")) == 0 ||
        strncmp(getbuf() + i, "HTTP/1.1", strlen("HTTP/1.1")) == 0) {
      request_.setVersion(string(getbuf() + i, strlen("HTTP/1.0")).data(),
                          strlen("HTTP/1.0"));
    }

    line_status_ = LINE_OK;
    break;
  }
}

void HttpPraser::prase_header() {
  LOG_DEBUG << "";
  while (check_index_ < read_index_) {
    prase_line();
    if (line_status_ != LINE_OK) {
      return;
    }

    int seprate;
    for (int i = start_line_; i < check_index_ ; ++i) {
      if (buf[i] == ':') {
        seprate = i;
        break;
      }
      if (i == read_index_ - 1) {
        line_status_ = LINE_OPEN;
        return;
      }
    }
    LOG_DEBUG << start_line_ << " " << seprate << " " << check_index_;
    request_.setHeader(
        string(getbuf() + start_line_, seprate - start_line_),
        string(getbuf() + seprate + 1, check_index_ - 3 - seprate));
    if (buf[check_index_] == '\r' && buf[check_index_ + 1] == '\n') {
      line_status_ = LINE_OK;
      return;
    } else {
      LOG_DEBUG << buf[check_index_ + 1] << "*" << buf[check_index_ + 2];
    }
  }
}

void HttpPraser::prase_body(size_t len) {
  if (start_line_ + len > read_index_) {
    line_status_ = LINE_BAD;
    return;
  }

  request_.addBody(getbuf() + start_line_, len);
}

HttpPraser::HTTP_CODE HttpPraser::prase() {
  LOG_DEBUG << "";
  check_index_ = PRASE_LINE;
  std::map<string, string>::iterator kv;
  while (check_index_ != read_index_) {
    // prase_line();
    switch (check_status_) {
      case PRASE_LINE:
        prase_questline();
        LOG_DEBUG << "*********" << line_status_;
        if (line_status_ == LINE_OK) {
          check_status_ = PRASE_HEAD;
        } else if (line_status_ == LINE_BAD) {
          return BAD_REQUEST;
        } else {
          return NO_REQUEST;
        }
        break;
      case PRASE_HEAD:
        prase_header();
        LOG_DEBUG << "*********" << line_status_;
        if (line_status_ == LINE_OK) {
          check_status_ = PRASE_BODY;
        } else if (line_status_ == LINE_BAD) {
          return BAD_REQUEST;
        } else {
          return NO_REQUEST;
        }
        break;
      case PRASE_BODY:

        kv = request_.getheaders().find("Content-Length");
        if (kv != request_.getheaders().end()) {
          std::string len = kv->second;
          int l = std::stoi(len);
          prase_body(l);
        }
        LOG_DEBUG << "*********" << line_status_;
        if (line_status_ == LINE_OK) {
          return GET_REQUEST;
        } else if (line_status_ == LINE_BAD) {
          return BAD_REQUEST;
        } else {
          return NO_REQUEST;
        }
        break;
      default:
        LOG_DEBUG << "**";
        break;
    }
  }
}

void HttpPraser::append(const char* data, size_t len) {
  // std::copy(data, data + len, begin());
  strncpy(begin(), data, len);
  read_index_ += len;
  LOG_DEBUG << getbuf();
  LOG_DEBUG << check_index_ << " " << read_index_;
}