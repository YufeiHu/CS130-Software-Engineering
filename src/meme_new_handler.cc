//
// Author: Junheng Hao, Yufei Hu, Qi Zeng
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_new_handler.h"

std::unique_ptr<RequestHandler> MemeNewHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> meme_new_handler(new MemeNewHandler(options));
  return meme_new_handler;
}

std::string MemeNewHandler::GetID(const Request& http_request) {
  int i = 0;
  while (i < http_request.url_full.length()) {
    if (i + 4 <= http_request.url_full.length() && http_request.url_full.substr(i, 4).compare("?id=") == 0) {
      return http_request.url_full.substr(i + 4, http_request.url_full.length());
    }
    i += 1;
  }
  return "";
}

std::unique_ptr<Reply> MemeNewHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  std::string id_str = GetID(http_request);
  std::string body;
  http_response->status_code = 200;

  if (id_str.length() == 0) {
    body = GenerateBody(http_request);
  } else {
    if (std::all_of(id_str.begin(), id_str.end(), ::isdigit)) {
      body = GenerateBodyUpdate(id_str);
    } else {
      BOOST_LOG_TRIVIAL(error) << "MemeNewHandler::HandleRequest->" \
                               << "response_code: 400 (id is illegal)";
      body = "Error: id is illegal!";
      http_response->status_code = 400;
    }
  }
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  http_response->content = body;
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "MemeNewHandler::HandleRequest-> response_code: " \
                          << http_response->status_code << ", " \
                          << http_response->mime_type << ", " \
                          << http_response->headers;
  return http_response;
}

std::string MemeNewHandler::GenerateBody(const Request& http_request) {
  std::string body;
  std::string path_full = options->root + options->path_meme_new.at(http_request.url_location + '/' + http_request.url_filename);
  std::ifstream file(path_full.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
  if (file.is_open()) {
    std::streampos size = file.tellg();
    char* membuff = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(membuff, size);
    file.close();
    body = std::string(membuff, size);
    delete membuff;
    BOOST_LOG_TRIVIAL(info) << "MemeNewHandler::GenerateBody-> " \
                            << "create.html is successfully sent";
  } else {
    BOOST_LOG_TRIVIAL(error) << "MemeNewHandler::GenerateBody->" \
                             << "response_code: 404 (cannot open the file in the given URL)";
    body = "Error: file cannot be opened!";
  }
  return body;
}

std::string MemeNewHandler::GenerateBodyUpdate(std::string id_str) {
  std::string body = ""
    "<head>\n"
      "<meta charset=\"utf-8\">\n"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n"
      "<meta name=\"description\" content=\"Meme Update Page\">\n"
      "<meta name=\"author\" content=\"Yufei Hu\">\n"

      "<title>Meme Update</title>\n"

      "<style type=\"text/css\">\n"
        "body {\n"
          "font-family: Arial, Helvetica, sans-serif;\n"
        "}\n"

        "* {\n"
          "box-sizing: border-box;\n"
        "}\n"

        "div.center {\n"
          "text-align: center;\n"
        "}\n"

        "div.warning {\n"
          "color: red;\n"
          "text-align: center;\n"
        "}\n"

        "input[type=text], select, textarea {\n"
          "width: 100%;\n"
          "padding: 12px;\n"
          "border: 1px solid #ccc;\n"
          "border-radius: 4px;\n"
          "box-sizing: border-box;\n"
          "margin-top: 6px;\n"
          "margin-bottom: 16px;\n"
          "resize: vertical;\n"
        "}\n"

        "input[type=submit] {\n"
          "background-color: #4CAF50;\n"
          "color: white;\n"
          "padding: 12px 20px;\n"
          "border: none;\n"
          "border-radius: 4px;\n"
          "cursor: pointer;\n"
        "}\n"

        "input[type=submit]:hover {\n"
          "background-color: #45a049;\n"
        "}\n"

        ".container {\n"
          "border-radius: 5px;\n"
          "background-color: #f2f2f2;\n"
          "padding: 20px;\n"
        "}\n"

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

    "<body>\n"
      "<div class=\"center\">\n"
        "<h1>Meme Update ID=" + id_str + "</h1>\n"
      "</div>\n"

      "<div class=\"warning\">\n"
        "<p id=\"warning\"></p>\n"
      "</div>\n"

      "<div class=\"container\">\n"
        "<form action=\"/meme/create\" method=\"post\" onsubmit=\"return checkInput(this);\">\n"
          "<label for=\"image\">Select an image</label>\n"
          "<select id=\"selectTemplate\" name=\"image\">\n"
            "<option value=\"Select a template...\">Select a template...</option>\n"
            "<option value=\"girl.gif\">girl.gif</option>\n"
            "<option value=\"eye.jpg\">eye.jpg</option>\n"
            "<option value=\"nuke.png\">nuke.png</option>\n"
          "</select><br>\n"
          "<label for=\"topText\">Image top text</label>\n"
          "<input id=\"topText\" type=\"text\" name=\"top\" placeholder=\"e.g. Girl\"><br>\n"
          "<label for=\"bottomText\">Image bottom text</label>\n"
          "<input id=\"bottomText\" type=\"text\" name=\"bottom\" placeholder=\"e.g. Look behind you\"><br>\n"
          "<input type=\"hidden\" name=\"id\" value=\"" + id_str + "\">\n"
          "<input id=\"submit\" type=\"submit\" value=\"Update\">\n"
        "</form>\n"
      "</div>\n"

      "<div class=\"pill-nav\">\n"
        "<a href=\"/meme/home\">Home</a>\n"
      "</div>\n"

      "<script type=\"text/javascript\">\n"
        "function checkInput(form) {\n"
          "let upperLimit = 100;\n"
          "let banned = [\'<\', \'>\', \'&\', \'=\'];\n"
          "let selectTemplate = document.getElementById(\"selectTemplate\").value;\n"
          "let topText = document.getElementById(\"topText\").value;\n"
          "let bottomText = document.getElementById(\"bottomText\").value;\n"
          "if (selectTemplate == \"Select a template...\") {\n"
            "document.getElementById(\"warning\").innerHTML = \"You must select a template!\";\n"
            "return false;\n"
          "}\n"
          "if (topText.length == 0) {\n"
            "document.getElementById(\"warning\").innerHTML = \"Input image title cannot be empty!\";\n"
            "return false;\n"
          "}\n"
          "if (topText.length > upperLimit) {\n"
            "document.getElementById(\"warning\").innerHTML = \"Input image title cannot exceed \" + upperLimit + \"!\";\n"
            "return false;\n"
          "}\n"
          "for (let i = 0; i < banned.length; i++) {\n"
            "if (topText.indexOf(banned[i]) > -1) {\n"
              "document.getElementById(\"warning\").innerHTML = \"Do not type \" + banned[i];\n"
              "return false;\n"
            "}\n"
          "}\n"

          "if (bottomText.length == 0) {\n"
            "document.getElementById(\"warning\").innerHTML = \"Input image comments cannot be empty!\";\n"
            "return false;\n"
          "}\n"
          "if (bottomText.length > upperLimit) {\n"
            "document.getElementById(\"warning\").innerHTML = \"Input image comments cannot exceed \" + upperLimit + \"!\";\n"
            "return false;\n"
          "}\n"
          "for (let i = 0; i < banned.length; i++) {\n"
            "if (bottomText.indexOf(banned[i]) > -1) {\n"
              "document.getElementById(\"warning\").innerHTML = \"Do not type \" + banned[i];\n"
              "return false;\n"
            "}\n"
          "}\n"

          "let topTextEsc = '';\n"
          "for (let i = 0; i < topText.length; i++) {\n"
            "if (topText.charAt(i) == \'" + "\\" + "\"" + "\') {\n"
              "topTextEsc = topTextEsc + 'U22';\n"
            "} else if(topText.charAt(i) == \'" + "\\" + "\'" + "\') {\n"
              "topTextEsc = topTextEsc + 'U27';\n"
            "} else {\n"
              "topTextEsc = topTextEsc + topText.charAt(i);\n"
            "}\n"
          "}\n"
          "document.getElementById(\"topText\").value = topTextEsc;\n"

          "let bottomTextEsc = \'\';\n"
          "for (let i = 0; i < bottomText.length; i++) {\n"
            "if (bottomText.charAt(i) == \'" + "\\" + "\"" + "\') {\n"
              "bottomTextEsc = bottomTextEsc + 'U22';\n"
            "} else if(bottomText.charAt(i) == \'" + "\\" + "\'" + "\') {\n"
              "bottomTextEsc = bottomTextEsc + 'U27';\n"
            "} else {\n"
              "bottomTextEsc = bottomTextEsc + bottomText.charAt(i);\n"
            "}\n"
          "}\n"
          "document.getElementById(\"bottomText\").value = bottomTextEsc;\n"
          "return true;\n"
        "}\n"
      "</script>\n"
    "</body>\n";
  return body;
}
