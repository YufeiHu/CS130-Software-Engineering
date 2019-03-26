//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "health_check_request_handler.h"

std::unique_ptr<RequestHandler> HealthCheckRequestHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> health_check_request_handler(new HealthCheckRequestHandler(options));
  return health_check_request_handler;
}

std::unique_ptr<Reply> HealthCheckRequestHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  http_response->content = "200 OK";
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "HealthCheckRequestHandler::HandleRequest-> response_code: " << http_response->status_code \
                          << ", " << http_response->mime_type << ", " << http_response->headers;
  return http_response;
}
