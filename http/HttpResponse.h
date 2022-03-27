#ifndef HTTP_HTTPRESPONSE_H
#define HTTP_HTTPRESPONSE_H

#include <map>
#include <memory>
#include <string>

#include "../net/TcpConnection.h"
#include "HttpRequest.h"
class HttpResponse {
  enum HttpStatusCode {
    kUnknown,
    k200Ok = 200,
    k301MovedPermanently = 301,
    k400BadRequest = 400,
    k404NotFounnd = 404,
  };

  HttpResponse(TcpConnectionPtr& conn) : state_(kUnknown), conn_(conn) {}

  void setStatusCode(HttpStatusCode state) { state_ = state; }
  void setVersion(std::string version) { version_ = version; }
  void addHeaders(const std::string& key, const std::string& value) {
    headers_[key] = value;
  }
  void setBody(const std::string&& body) {
    body_ = std::move(body);
    addContentSize(body_.size());
  }

  void addContentSize(size_t size) {
    size_ = size;
    addHeaders("Content-Size", std::to_string(size));
  }
  void addContentType(std::string type) { addHeaders("Content-Type", type); }

 private:
  HttpStatusCode state_;
  std::string version_;
  std::map<std::string, std::string> headers_;
  std::string body_;
  std::shared_ptr<HttpRequest> request_;
  TcpConnectionPtr conn_;
  size_t size_;
};

#endif