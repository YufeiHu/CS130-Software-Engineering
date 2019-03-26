//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_delete_handler.h"

int MemeDeleteHandler::GetParsedId(std::string body) {
  std::string prefix;
  bool flag = false;
  std::string id;
  for (int i = 0; i < body.length(); i++) {
    if (!flag) {
      if (!isdigit(body[i])) {
        prefix += body[i];
        continue;
      }
      if (prefix.compare("/meme/delete/id=") != 0) {
        return -1;
      } else {
        flag = true;
      }
    }
    if (isdigit(body[i])) {
      id += body[i];
    } else {
      return -1;
    }
  }
  if (!flag)
    return -1;
  int meme_id = std::stoi(id);
  return meme_id;
}

std::unique_ptr<RequestHandler> MemeDeleteHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> meme_delete_handler(new MemeDeleteHandler(options));
  return meme_delete_handler;
}

std::unique_ptr<Reply> MemeDeleteHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";

  int id = GetParsedId(http_request.url_full);
  bool id_found = false;

  // check id exist
  std::tuple<int, std::string, std::string, std::string, std::string> ans = MemeHandlerManager::Access().GetMeme(id);
  std::string image_name = std::get<3>(ans);
 
  if (image_name.length() != 0) {
    MemeHandlerManager::Access().DeleteMeme(id);
    http_response->status_code = 200;
    http_response->content = GenerateBody(true, id);
    http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
    BOOST_LOG_TRIVIAL(info) << "MemeDeleteHandler::HandleRequest-> response = \n" \
                            << http_response->status_code << ", " \
                            << http_response->mime_type << ", " \
                            << http_response->headers;
  } else {
    http_response->status_code = 400;
    http_response->content = GenerateBody(false, id);
    http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
    BOOST_LOG_TRIVIAL(info) << "MemeDeleteHandler::HandleRequest-> response = \n" \
                            << http_response->status_code << ", " \
                            << http_response->mime_type << ", " \
                            << http_response->headers;
  }
  return http_response;
}

std::string MemeDeleteHandler::GenerateBody(bool is_deleted, int image_id) {
    std::string head_body = "";
    if (is_deleted)
      head_body = " [Info] The meme ID:"+ std::to_string(image_id) + " has been deleted!";
    else
      head_body += " [Error] The meme ID:"+ std::to_string(image_id) + " cannot be deleted!";

    std::string body = ""
      "<html>\n"
        "<head>\n"
          "<style>\n"
            ".pill-nav a {\n"
              "display: block;\n"
              "color: black;\n"
              "padding: 14px;\n"
              "text-decoration: none;\n"
              "font-size: 17px;\n"
              "border-radius: 5px;\n"
              "text-align: center;\n"
              "background-color: #ddd;\n"
              "margin-top: 5px;\n"
            "}\n"
            ".pill-nav a:hover {\n"
              "background-color: dodgerblue;\n"
              "color: white;\n"
            "}\n"
            ".pill-nav a.active {\n"
              "background-color: dodgerblue;\n"
              "color: white;\n"
            "}\n"
          "</style>\n"
        "</head>\n"
        "<p1>"+ head_body + "</p1>\n"
        "<div class=\"pill-nav\">\n"
          "<a href=\"/meme/home\">Home</a>\n"
        "</div>\n"
      "</html>";

  return body;
}
