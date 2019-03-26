//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "file_handler.h"

std::unique_ptr<RequestHandler> FileHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> file_handler(new FileHandler(options));
  return file_handler;
}

std::unique_ptr<Reply> FileHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->mime_type = http_request.url_extension;
  std::string path_full = options->root + options->path_file.at(http_request.url_location) + http_request.url_filename;
  std::ifstream file(path_full.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
  if (file.is_open()) {
    if (http_request.url_extension.compare("md") == 0 || http_request.url_extension.compare("MD") == 0) {
      file.close();
      file.open(path_full);
      std::ostringstream o;
      markdown::Document d;
      if (d.read(file)) {
        d.write(o);
        file.close();
        http_response->status_code = 200;
        http_response->content = o.str();
        http_response->headers = "Content-Type: " + RequestHandlerUtil::ext_to_type[http_request.url_extension] + "\r\n";
        http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
      } else {
        BOOST_LOG_TRIVIAL(error) << "FileHandler::HandleRequest->" \
                                 << "response_code: 404 (cannot open the file in the given URL)";
        RequestHandlerUtil::HandleRequestError(*http_response, 404, "Error: cannot open the file in the given URL!\r\n\r\n");
      }
    } else if (RequestHandlerUtil::ext_to_type.find(http_request.url_extension) != RequestHandlerUtil::ext_to_type.end()) {
      std::streampos size = file.tellg();
      char* membuff = new char[size];
      file.seekg(0, std::ios::beg);
      file.read(membuff, size);
      file.close();
      http_response->status_code = 200;
      http_response->content = std::string(membuff, size);
      http_response->headers = "Content-Type: " + RequestHandlerUtil::ext_to_type[http_request.url_extension] + "\r\n";
      http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
      delete membuff;
    } else {
      BOOST_LOG_TRIVIAL(error) << "FileHandler::HandleRequest->" \
                               << "response_code: 404 (unsupported file extension)";
      RequestHandlerUtil::HandleRequestError(*http_response, 404, "Error: unsupported file extension!\r\n\r\n");
    }
    BOOST_LOG_TRIVIAL(info) << "FileHandler::HandleRequest-> response_code: " << http_response->status_code \
                            << ", " << http_response->headers;
  } else {
    BOOST_LOG_TRIVIAL(error) << "FileHandler::HandleRequest->" \
                             << "response_code: 404 (cannot open the file in the given URL)";
    RequestHandlerUtil::HandleRequestError(*http_response, 404, "Error: cannot open the file in the given URL!\r\n\r\n");
  }
  return http_response;
}
