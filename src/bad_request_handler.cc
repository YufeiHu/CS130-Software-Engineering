//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "bad_request_handler.h"

std::unique_ptr<RequestHandler> BadRequestHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> bad_request_handler(new BadRequestHandler(options));
  return bad_request_handler;
}

std::unique_ptr<Reply> BadRequestHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  BOOST_LOG_TRIVIAL(error) << "BadRequestHandler::HandleRequest->response_code: 400 (bad request)";
  RequestHandlerUtil::HandleRequestError(*http_response, 400, "Error: bad request!\r\n\r\n");
  return http_response;
}
