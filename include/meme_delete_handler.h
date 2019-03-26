//
// Author: Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_DELETE_HANDLER
#define MEME_DELETE_HANDLER

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "meme_handler_manager.h"
#include "request_handler.h"
#include "request_handler_util.h"

class MemeDeleteHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  MemeDeleteHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  int GetParsedId(std::string body);
  std::string GenerateBody(bool is_deleted, int image_id);
};

#endif
