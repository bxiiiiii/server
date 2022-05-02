#ifndef HTTP_HTTPRESPONSE_H
#define HTTP_HTTPRESPONSE_H

#include <map>
#include <memory>
#include <string>

#include "../net/TcpConnection.h"
#include "HttpRequest.h"
class HttpResponse {
 public:
  enum HttpStatusCode {
    kUnknown,
    k200Ok = 200,
    k301MovedPermanently = 301,
    k400BadRequest = 400,
    k404NotFounnd = 404,
  };

  HttpResponse(const TcpConnectionPtr& conn) : state_(kUnknown), conn_(conn) {}

  void setConnection(const string& connection) {
    if ("close" == connection) {
      closeConnection_ = true;
    } else {
      closeConnection_ = false;
    }
  }
  void setStatusMessage(const string& mes) { statusMessage_ = mes; }
  void setStatusCode(HttpStatusCode state) { state_ = state; }
  void setVersion(std::string version) { version_ = version; }
  void addHeaders(const std::string& key, const std::string& value) {
    headers_[key] = value;
  }
  void setBody(const std::string&& body) { body_ = std::move(body); }

  void addContentType(std::string type) { addHeaders("Content-Type", type); }

  void appendToBuffer(Buffer* output) {
    char buf[32];
    snprintf(buf, sizeof buf, "%s %d ", version_, state_);
    output->append(buf);
    output->append(statusMessage_);
    output->append("\r\n");

    if (closeConnection_) {
      output->append("Connection: close\r\n");
    } else {
      snprintf(buf, sizeof buf, "Content-Length: %d\r\n", body_.size());
      output->append(buf);
      output->append("Connection: Keep-Alive\r\n");
    }

    for (auto& header : headers_) {
      output->append(header.first);
      output->append(": ");
      output->append(header.second);
      output->append("\r\n");
    }

    output->append("\r\n");
    output->append(body_);
  }

  bool closeConnection() const { return closeConnection_; }

 private:
  HttpStatusCode state_;
  std::string version_;
  std::map<std::string, std::string> headers_;
  std::string body_;
  std::shared_ptr<HttpRequest> request_;
  TcpConnectionPtr conn_;
  string statusMessage_;
  bool closeConnection_;
};

#endif