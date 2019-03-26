//
// Author: Junheng Hao, Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_LIST_HANDLER
#define MEME_LIST_HANDLER

#include <tuple>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "request_handler.h"
#include "request_handler_util.h"
#include "meme_handler_manager.h"
#include "search_algo_factory.h"

class MemeListHandler : public RequestHandler {
 public:
  static std::unique_ptr<MemeListHandler> create(const ConfigOptions* options);
  MemeListHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  std::string GenerateBody(std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> ans);
  std::string GetQuery(const Request& http_request);
  void EscapeHTMLCharacters(std::string& data);
};

#endif
