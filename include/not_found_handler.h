//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef NOT_FOUND_HANDLER
#define NOT_FOUND_HANDLER

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"

class NotFoundHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  NotFoundHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
};

#endif
