//
// Author: Junheng Hao, Yufei Hu, Qi Zeng
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_create_handler.h"

std::unique_ptr<RequestHandler> MemeCreateHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> meme_create_handler(new MemeCreateHandler(options));
  return meme_create_handler;
}

std::unique_ptr<Reply> MemeCreateHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  if (http_request.method.compare("POST") != 0) {
    BOOST_LOG_TRIVIAL(error) << "MemeCreateHandler::HandleRequest->" \
                             << "ERROR(400): request method is illegal";
    RequestHandlerUtil::HandleRequestError(*http_response, 400, "Error: request method is illegal!\r\n\r\n");
    return http_response;
  }
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";

  std::string request_body = http_request.body;
  BOOST_LOG_TRIVIAL(info) << "MemeCreateHandler::HandleRequest-> request body = \n" \
                          << http_request.body;
  std::string delimiter1 = "&";
  std::string delimiter2 = "=";
  size_t pos1 = 0;
  size_t pos2 = 0;
  std::string token1;
  std::vector<std::string> token2;
  
  request_body += "&";
  while ((pos1 = request_body.find(delimiter1)) != std::string::npos) {
    token1 = request_body.substr(0, pos1);
    if ((pos2 = token1.find(delimiter2)) != std::string::npos) {
      token2.push_back(token1.substr(pos2 + 1, token1.length()));
    } else {
      token2.push_back("");
    }
    request_body.erase(0, pos1 + 1);
  }
  std::string image_name = token2[0];
  std::string top_text = "";
  std::string bottom_text = "";
  std::string id_str = "";
  if (token2.size() == 2) {
    top_text = token2[1]; 
  } else if (token2.size() == 3) {
    top_text = token2[1];
    bottom_text = token2[2];
  } else if (token2.size() == 4) {
    top_text = token2[1];
    bottom_text = token2[2];
    id_str = token2[3];
  }

  std::string body;
  std::string image_url = options->path_file.at("/img") + image_name;
  if (id_str.length() == 0) {
    int meme_id = MemeHandlerManager::Access().InsertMeme(top_text.c_str(), bottom_text.c_str(), image_name.c_str(), image_url.c_str());
    if (meme_id == -1) {
      BOOST_LOG_TRIVIAL(error) << "MemeCreateHandler::HandleRequest->" \
                               << "response_code: 404 (cannot insert)";
      RequestHandlerUtil::HandleRequestError(*http_response, 404, "Error: cannot open the file in the given URL!\r\n\r\n");
    } else {
      body = GenerateBody("created", meme_id);
      http_response->content = body;
      http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
      BOOST_LOG_TRIVIAL(info) << "MemeCreateHandler::HandleRequest-> response_code: " \
                              << http_response->status_code << ", " \
                              << http_response->mime_type << ", " \
                              << http_response->headers;
    }
  } else {
    BOOST_LOG_TRIVIAL(info) << "MemeCreateHandler::HandleRequest-> response_code: 200 (id_str = " \
                            << id_str << ")";
    MemeHandlerManager::Access().EditMeme(std::stoi(id_str), top_text.c_str(), bottom_text.c_str(), image_name.c_str(), image_url.c_str());
    body = GenerateBody("updated", std::stoi(id_str));
    http_response->content = body;
    http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
    BOOST_LOG_TRIVIAL(info) << "MemeCreateHandler::HandleRequest-> response_code: " \
                            << http_response->status_code << ", " \
                            << http_response->mime_type << ", " \
                            << http_response->headers;
  }
  return http_response;
}

std::string MemeCreateHandler::GenerateBody(std::string tag, int meme_id) {
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
      "<p1>Your meme is successfully " + tag + "</p1>\n"
      "Its ID is: <a href=\"/meme/view/id=" \
      + std::to_string(meme_id) + "\">" + std::to_string(meme_id) + "</a>\n"
      "<div class=\"pill-nav\">\n"
        "<a href=\"/meme/home\">Home</a>\n"
      "</div>\n"
    "</html>";
  return body;
}
