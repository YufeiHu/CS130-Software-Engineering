//
// Author: JMAC
// ~~~~~~~~~~~~
//

#ifndef PROXY_HANDLER
#define PROXY_HANDLER

#include <string>
#include <boost/log/trivial.hpp>

#include "request_handler.h"

using boost::asio::ip::tcp;

class ProxyHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  ProxyHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
  std::unique_ptr<Reply> Redirect(std::string destination, std::string request_query);
};

#endif
