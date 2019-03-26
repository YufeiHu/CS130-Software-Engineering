//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_VIEW_HANDLER
#define MEME_VIEW_HANDLER

#include <tuple>
#include <iostream>
#include <unordered_map>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"
#include "request_handler_util.h"
#include "meme_handler_manager.h"

class MemeViewHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  MemeViewHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) {}
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  void EscapeHTMLCharacters(std::string& data);
  std::string GenerateBody(std::string fig, std::string top, std::string bottom);
  int GetParsedId(std::string body);
};

#endif
