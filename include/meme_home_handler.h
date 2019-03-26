//
// Author: Yufei Hu, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_HOME_HANDLER
#define MEME_HOME_HANDLER

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"

class MemeHomeHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  MemeHomeHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  std::string GenerateBody(const Request& http_request);
};

#endif
