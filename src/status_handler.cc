//
// Author: Junheng Hao, Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "status_handler.h"

std::unique_ptr<RequestHandler> StatusHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> status_handler(new StatusHandler(options));
  return status_handler;
}

std::unique_ptr<Reply> StatusHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  std::string body = StatusHandler::GenerateBody();
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  http_response->content = body;
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "StatusHandler::Redirect-> response_code: " \
                          << http_response->status_code << ", " \
                          << http_response->mime_type << ", " \
                          << http_response->headers;
  return http_response;
}

std::string StatusHandler::GenerateBody(){
  int numRequests = ServerStatus::getInstance().GetNumRequest();
  int numHandlers = ServerStatus::getInstance().GetNumHandler();
  BOOST_LOG_TRIVIAL(info) << "StatusHandler::GenerateBody-> " \
                          << "Get number of requests:" << numRequests;
  BOOST_LOG_TRIVIAL(info) << "StatusHandler::GenerateBody-> " \
                          << "Get number of handlers:" << numHandlers;
  std::vector<std::pair<std::string, std::string>> handler_names = ServerStatus::getInstance().getHandlers();
  std::vector<std::pair<std::string, int>> requests_logs = ServerStatus::getInstance().getRequests();

  // add request numbers & logs
  std::string body = "<html>";
  body += "<head><title>Server Status</title></head>";
  body += "<h2>Requests</h2>";
  body += "<p> Total Number of Requests: " + std::to_string(numRequests) + "</p>" ;
  body += "<table><tr><th>URL Requested</th><th>Response Code</th></tr>";
  for (int i = 0; i < requests_logs.size(); i++) {
  	body += "<tr><td>" + requests_logs[i].first + "</td><td>" + std::to_string(requests_logs[i].second) + "</td></tr>"; 
  }
  body += "</table><br>";

  // add handler names
  body += "<h2>Handlers</h2>";
  body += "<p> Total Number of Handlers: " + std::to_string(numHandlers) + "</p>" ;
  body += "<table><tr><th>URI Prefix</th><th>Request Handler</th></tr>";
  for (int i = 0; i < handler_names.size(); i++) {
  	body += "<tr><td>" + handler_names[i].first + "</td><td>" + handler_names[i].second + "</td></tr>"; 
  }
  body += "</table><br>";
  body += "</html>";
  return body;
}
