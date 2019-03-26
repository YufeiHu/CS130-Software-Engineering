//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_util.h"

std::string Reply::GetResponse() {
  return RequestHandlerUtil::status_code_to_line[status_code] + headers + "\r\n" + content;
}

std::unordered_map<std::string, std::string> RequestHandlerUtil::ext_to_type = {
  {"md", "text/html"},
  {"MD", "text/html"},
  {"html", "text/html"},
  {"HTML", "text/html"},
  {"txt", "text/plain"},
  {"TXT", "text/plain"},
  {"jpg", "image/jpeg"},
  {"JPG", "image/jpeg"},
  {"png", "image/png"},
  {"PNG", "image/png"},
  {"gif", "image/gif"},
  {"GIF", "image/gif"},
  {"pdf", "application/pdf"},
  {"PDF", "application/pdf"},
  {"zip", "application/zip"},
  {"ZIP", "application/zip"}
};

std::unordered_map<int, std::string> RequestHandlerUtil::status_code_to_line = {
  {200, "HTTP/1.1 200 OK\r\n"},
  {400, "HTTP/1.1 400 Bad Request\r\n"},
  {404, "HTTP/1.1 404 Not Found\r\n"}
};

void RequestHandlerUtil::HandleRequestError(Reply& http_response,
                                            int error_code,
                                            std::string error_body) {
  http_response.status_code = error_code;
  http_response.mime_type = "html";
  http_response.headers = "Content-Type: text/html\r\n";
  http_response.headers += GetContentLength(error_body);
  http_response.content = error_body;
}

std::string RequestHandlerUtil::GetContentLength(std::string response_body) {
  std::string length;
  std::ostringstream temp;
  temp << ((int) response_body.size());
  length = temp.str();
  return "Content-Length: " + length + "\r\n";
}
