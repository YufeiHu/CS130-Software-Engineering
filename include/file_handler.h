//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"

class FileHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  FileHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
};

#endif
