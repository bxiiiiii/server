#ifndef HTTP_HTTPREQUEST_H
#define HTTP_HTTPREQUEST_H

#include <cstring>
#include <map>
#include <string>

#include "../base/Logging.h"

// namespace bxx {
// namespace net {
using std::string;
class HttpRequest {
 public:
  enum Method {
    kInvalid,
    kGet,
    kHead,
    kPost,
    // kPUT,    for http/1.1
    // kDelete,
    // kConnect,
    // kOptions,
    // kTrace,
  };

  HttpRequest() : method_(kInvalid), version_(0x00){};

  bool setMethod(const char* start, const size_t len) {
    if (strncmp(start, "GET", len) == 0) {
      method_ = kGet;
    } else if (strncmp(start, "HEAD", len) == 0) {
      method_ = kHead;
    } else if (strncmp(start, "POST", len) == 0) {
      method_ = kPost;
    } else {
      method_ = kInvalid;
    }
    return method_ != kInvalid;
  }

  bool setVersion(const char* version, const size_t len) {
    if (strncmp(version, "HTTP/1.0", len) == 0) {
      version_ = 0x10;
    } else if (strncmp(version, "HTTP/1.1", len) == 0) {
      version_ = 0x11;
    } else {
      version_ = 0x00;
    }
    return version_ != 0x00;
  }

  Method method() const { return method_; }
  const char* methodToString() const {
    const char* result = "UNKNOWN";
    switch (method_) {
      case kGet:
        result = "GET";
        break;
      case kHead:
        result = "HEAD";
        break;
      case kPost:
        result = "POST";
        break;
      default:
        break;
    }
    return result;
  }

  void setPath(const char* start, const size_t len) {
    path_.assign(start, len);
  }
  const string& path() const { return path_; }
  void setQuery(const char* start, const size_t len) {
    query_.assign(start, len);
  }
  const string& query() const { return query_; }
  void addBody(const char* start, size_t len) { body_.append(start, len); }
  const string& body() const { return body_; }
  void setHeader(string key, string value) {
    headers_.insert({key.data(), value.data()});
  }

  std::map<string, string> getheaders() { return headers_; }
  string getHeader(const string& str) const {
    string result;
    auto it = headers_.find(str);
    if (it != headers_.end()) {
      result = it->second;
    }
    return result;
  }
  void printrequest() {
    LOG_DEBUG << methodToString();
    LOG_DEBUG << version_;
    LOG_DEBUG << path_;
    for (auto i : headers_) {
      LOG_DEBUG << i.first << ":" << i.second;
    }
    LOG_DEBUG << body_;
  }

 private:
  Method method_;
  unsigned int version_;
  string path_;
  string query_;
  string body_;
  std::map<string, string> headers_;
};

// }  // namespace net
// }  // namespace bxx

#endif