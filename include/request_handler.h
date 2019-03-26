//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "markdown.h"
#include "config_options.h"
#include "request_handler_util.h"

using boost::asio::ip::tcp;

class RequestHandler {
 public:
  RequestHandler(const ConfigOptions* options_upstream) : options(options_upstream) { }
  const ConfigOptions* options;
  virtual ~RequestHandler() {}
  virtual std::unique_ptr<Reply> HandleRequest(const Request& http_request) = 0;
};

#endif
