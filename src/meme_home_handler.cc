//
// Author: Yufei Hu, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_home_handler.h"

std::unique_ptr<RequestHandler> MemeHomeHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> meme_home_handler(new MemeHomeHandler(options));
  return meme_home_handler;
}

std::unique_ptr<Reply> MemeHomeHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  std::string body = GenerateBody(http_request);
  http_response->content = body;
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "MemeHomeHandler::HandleRequest-> response_code: " \
                          << http_response->status_code << ", " \
                          << http_response->mime_type << ", " \
                          << http_response->headers;
  return http_response;
}

std::string MemeHomeHandler::GenerateBody(const Request& http_request) {
  std::string body;
  std::string path_full = options->root + options->path_meme_home.at(http_request.url_full);
  std::ifstream file(path_full.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
  if (file.is_open()) {
    std::streampos size = file.tellg();
    char* membuff = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(membuff, size);
    file.close();
    body = std::string(membuff, size);
    delete membuff;
    BOOST_LOG_TRIVIAL(info) << "MemeHomeHandler::GenerateBody-> " \
                            << "home.html is successfully sent";
  } else {
    BOOST_LOG_TRIVIAL(error) << "MemeHomeHandler::GenerateBody->" \
                             << "response_code: 404 (cannot open the file in the given URL)";
    body = "Error: file cannot be opened!";
  }
  return body;
}
