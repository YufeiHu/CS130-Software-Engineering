//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "echo_handler.h"

std::unique_ptr<RequestHandler> EchoHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> echo_handler(new EchoHandler(options));
  return echo_handler;
}

std::unique_ptr<Reply> EchoHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  http_response->content = http_request.all;
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "EchoHandler::HandleRequest-> response_code: " << http_response->status_code \
                          << ", " << http_response->mime_type << ", " << http_response->headers;
  return http_response;
}
