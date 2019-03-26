//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "not_found_handler.h"

std::unique_ptr<RequestHandler> NotFoundHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> not_found_handler(new NotFoundHandler(options));
  return not_found_handler;
}

std::unique_ptr<Reply> NotFoundHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  BOOST_LOG_TRIVIAL(error) << "NotFoundHandler::HandleRequest->" \
                           << "response_code: 404 (page not found)";
  RequestHandlerUtil::HandleRequestError(*http_response, 404, "<html><body><h1>Not Found</h1></body></html>\r\n\r\n");
  return http_response;
}
