//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef HEALTH_CHECK_REQUEST_HANDLER
#define HEALTH_CHECK_REQUEST_HANDLER

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"

class HealthCheckRequestHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  HealthCheckRequestHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
};

#endif