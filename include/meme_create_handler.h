//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_CREATE_HANDLER
#define MEME_CREATE_HANDLER

#include <vector>
#include <string>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"
#include "meme_handler_manager.h"

class MemeCreateHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  MemeCreateHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  std::string GenerateBody(std::string tag, int meme_id);
};

#endif
