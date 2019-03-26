//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_REQUEST_HANDLER
#define MEME_REQUEST_HANDLER

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"

class MemeNewHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  MemeNewHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::string GetID(const Request& http_request);
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  std::string GenerateBody(const Request& http_request);
  std::string GenerateBodyUpdate(std::string id_str);
};

#endif
