//
// Author: Qi Zeng, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_view_handler.h"

int MemeViewHandler::GetParsedId(std::string body) {
  std::string prefix;
  bool flag = false;
  std::string id;
  for (int i = 0; i < body.length(); i++) {
    if (!flag) {
      if (!isdigit(body[i])) {
        prefix += body[i];
        continue;
      }
      if (prefix.compare("/meme/view/id=") != 0) {
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

std::unique_ptr<RequestHandler> MemeViewHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> meme_view_handler(new MemeViewHandler(options));
  return meme_view_handler;
}

std::unique_ptr<Reply> MemeViewHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  int id = GetParsedId(http_request.url_full);

  std::tuple<int, std::string, std::string, std::string, std::string> ans = MemeHandlerManager::Access().GetMeme(id);
  int meme_id = std::get<0>(ans);
  std::string top_text = std::get<1>(ans);
  std::string bottom_text = std::get<2>(ans);
  std::string image_name = std::get<3>(ans);
  std::string image_url = std::get<4>(ans);

  if (image_name.length() == 0) {
    BOOST_LOG_TRIVIAL(error) << "MemeViewHandler::HandleRequest->" \
                             << "response_code: 404 (cannot find id = " << id << ")";
    RequestHandlerUtil::HandleRequestError(*http_response, 404, "Error: cannot find id!\r\n\r\n");
  } else {
    http_response->content = GenerateBody(image_name, top_text, bottom_text);
    http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
    BOOST_LOG_TRIVIAL(info) << "MemeViewHandler::HandleRequest-> response_code: " \
                            << http_response->status_code << ", " \
                            << http_response->mime_type << ", " \
                            << http_response->headers;
  }
  return http_response;
}

void MemeViewHandler::EscapeHTMLCharacters(std::string& data) {
  std::string buffer;
  int data_length = (int)data.size();
  buffer.reserve(data_length);
  int i = 0;
  std::unordered_map<std::string, std::string> mark_to_entity = {
    {"U22", "&quot;"},
    {"U27", "&apos;"},
    {"%21", "&excl;"},
    {"%5B", "&lsqb;"},
    {"%5D", "&rsqb;"},
    {"%3B", "&semi;"},
    {"%3A", "&colon;"},
    {"%2F", "&sol;"},
    {"%2C", "&comma;"},
    {"%7C", "&verbar;"},
    {"%7B", "&lcub;"},
    {"%7D", "&rcub;"},
    {"%5C", "&bsol;"},
    {"%40", "&commat;"},
    {"%23", "&num;"},
    {"%24", "&dollar;"},
    {"%25", "&percnt;"},
    {"%5E", "&Hat;"},
    {"%28", "&lpar;"},
    {"%29", "&rpar;"},
    {"+", "&nbsp;"}
  };
  while (i < data_length) {
    bool mark_found = false;
    for (auto it : mark_to_entity) {
      int mark_length = it.first.length();
      if (i + mark_length <= data_length && data.substr(i, mark_length).compare(it.first) == 0) {
        buffer.append(it.second);
        i += mark_length;
        mark_found = true;
        break;
      }
    }
    if (mark_found == false) {
      buffer.append(&data[i], 1);
      i += 1;
    }
  }
  data.swap(buffer);
}

std::string MemeViewHandler::GenerateBody(std::string fig, std::string top, std::string bottom) {
  EscapeHTMLCharacters(top);
  EscapeHTMLCharacters(bottom);
  BOOST_LOG_TRIVIAL(info) << "MemeViewHandler::GenerateBody-> top = " << top;
  BOOST_LOG_TRIVIAL(info) << "MemeViewHandler::GenerateBody-> bottom = " << bottom;
  std::string body = ""
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
      "body { display: inline-block; position: relative; }\n"
      "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }\n"
      "#top { top: 0; }\n"
      "#bottom { bottom: 50px; }\n"
    "</style>\n"
    "<body>\n"
      "<img src=\"/img/" + fig + "\">\n"
      "<span id=\"top\">" + top + "</span>\n"
      "<span id=\"bottom\">" + bottom + "</span>\n"
      "<div class=\"pill-nav\">\n"
        "<a href=\"/meme/home\">Home</a>\n"
      "</div>\n"
    "</body>";
  return body;
}
