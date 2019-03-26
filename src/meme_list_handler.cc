//
// Author: Junheng Hao, Yufei Hu, Zhechen Xu, Qi Zeng
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_list_handler.h"

std::unique_ptr<MemeListHandler> MemeListHandler::create(const ConfigOptions* options) {
  std::unique_ptr<MemeListHandler> meme_list_handler(new MemeListHandler(options));
  return meme_list_handler;
}

std::string MemeListHandler::GetQuery(const Request& http_request) {
  int i = 0;
  while (i < http_request.url_full.length()) {
    if (i + 3 <= http_request.url_full.length() && http_request.url_full.substr(i, 3).compare("?q=") == 0) {
      return http_request.url_full.substr(i + 3, http_request.url_full.length());
    }
    i += 1;
  }
  return "";
}

void MemeListHandler::EscapeHTMLCharacters(std::string& data) {
  std::string buffer;
  int data_length = (int)data.size();
  buffer.reserve(data_length);
  int i = 0;
  std::unordered_map<std::string, std::string> mark_to_entity = {
    {"U22", "\""},
    {"U27", "\'"},
    {"%21", "!"},
    {"%5B", "["},
    {"%5D", "]"},
    {"%3B", ";"},
    {"%3A", ":"},
    {"%2F", "/"},
    {"%2C", ","},
    {"%7C", "|"},
    {"%7B", "{"},
    {"%7D", "}"},
    {"%5C", "\\"},
    {"%40", "@"},
    {"%23", "#"},
    {"%24", "$"},
    {"%25", "%"},
    {"%5E", "^"},
    {"%28", "("},
    {"%29", ")"},
    {"+", " "}
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

std::unique_ptr<Reply> MemeListHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  std::string query = GetQuery(http_request);
  BOOST_LOG_TRIVIAL(info) << "MemeListHandler::HandleRequest-> " \
                          << "query = " << query;
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> ans;
  if (query.length() == 0) {
    ans = MemeHandlerManager::Access().GetMemeAll();
  } else {
    SearchAlgo* search_algo = SearchAlgoFactory::GetAlgo("Jaro Winkler");
    std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> meme_info = MemeHandlerManager::Access().GetMemeAll();
    std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>::iterator it;
    std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> meme_info_escaped;
    for (it = meme_info.begin(); it != meme_info.end(); it++) {
      int meme_id = std::get<0>(*it);
      std::string top_text = std::get<1>(*it);
      std::string bottom_text = std::get<2>(*it);
      std::string image_name = std::get<3>(*it);
      std::string image_url = std::get<4>(*it);
      EscapeHTMLCharacters(top_text);
      EscapeHTMLCharacters(bottom_text);
      EscapeHTMLCharacters(image_name);
      EscapeHTMLCharacters(image_url);
      meme_info_escaped.push_back(std::make_tuple(meme_id, top_text, bottom_text, image_name, image_url));
    }
    EscapeHTMLCharacters(query);
    ans = search_algo->GetRank(query, meme_info_escaped);
  }
  std::string body = GenerateBody(ans);
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  http_response->content = body;
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "MemeListHandler::HandleRequest-> response_code: " \
                          << http_response->status_code << ", " \
                          << http_response->mime_type << ", " \
                          << http_response->headers;
  return http_response;
}

std::string MemeListHandler::GenerateBody(std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> ans) {
  std::string body = ""
    "<html>\n"
      "<head>\n"
        "<style>\n"
          "body {\n"
            "font-family: Arial, Helvetica, sans-serif;\n"
          "}\n"
          "* {\n"
            "box-sizing: border-box;\n"
          "}\n"
          "div.warning {\n"
            "color: red;\n"
            "text-align: center;\n"
          "}\n"
          "div.center {\n"
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
            "margin-top: 10px;\n"
            "border-radius: 5px;\n"
            "background-color: #f2f2f2;\n"
            "padding: 20px;\n"
          "}\n"
          "table {\n"
            "font-family: arial, sans-serif;\n"
            "border-collapse: collapse;\n"
            "width: 100%;\n"
          "}\n"
          "td, th {\n"
            "border: 1px solid #dddddd;\n"
            "text-align: left;\n"
            "padding: 8px;\n"
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
        "<script type=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/jquery/1.6.2/jquery.min.js\"></script>\n"
        "<h1><center>Meme List</center></h1>\n"
      "</head>\n"
    "<div class=\"warning\">\n"
      "<p id=\"warning\"></p>\n"
    "</div>\n"
    "<p>Total Number of Memes: " + std::to_string(ans.size()) + "</p>\n"
      "<div class=\"wrapper\">\n"
        "<div class=\"profile\">\n"
          "<table id=\"memedata\">\n"
            "<thead>\n"
              "<th>Meme ID</th>\n"
              "<th>Top Text</th>\n"
              "<th>Bottom Text</th>\n"
              "<th>Image Name</th>\n"
              "<th>Image URL</th>\n"
              "<th>Choose to edit</th>\n"
              "<th>Choose to delete</th>\n"
            "</thead>\n"
            "<tbody>\n";
  
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>::iterator it;
  for (it = ans.begin(); it != ans.end(); it++)
    body += "<tr><td>" + std::to_string(std::get<0>(*it)) \
          + "</td><td>" + std::get<1>(*it) \
          + "</td><td>" + std::get<2>(*it) \
          + "</td><td><a href=\"/meme/view/id=" \
          + std::to_string(std::get<0>(*it)) + "\">" + std::get<3>(*it) + "</a>" \
          + "</td><td><a href=\"" \
          + std::get<4>(*it) + "\">" + std::get<4>(*it) + "</a>" \
          + "</td><td><a href=\"/meme/new?id=" \
          + std::to_string(std::get<0>(*it)) + "\"> Edit </a>" \
          + "</td><td><a href=\"/meme/delete/id=" \
          + std::to_string(std::get<0>(*it)) + "\" onclick=\"return confirm('Are you sure?')\"> Delete </a>" \
          + "</td></tr>\n";
  
  body +=  "</tbody>\n"
          "</table>\n"
        "</div>\n"
      "</div>\n"
      "<div class=\"pill-nav\">\n"
        "<a href=\"/meme/home\">Home</a>\n"
      "</div>\n"
      "<div action=\"/meme/list\" class=\"container\" onsubmit=\"return checkInput(this);\">\n"
        "<form method=\"get\">\n"
          "<input type=\"text\" name=\"q\" id=\"query\">\n"
          "<input type=\"submit\" value=\"Search\">\n"
        "</form>\n"
      "</div>\n"
     "<script type=\"text/javascript\">\n"
        "function checkInput(form) {\n"
          "let upperLimit = 100;\n"
          "let banned = ['<', '>', '&', '='];\n"
          "let query = document.getElementById(\"query\").value;\n"
          "if (query.length == 0) {\n"
            "document.getElementById(\"warning\").innerHTML = \"Query cannot be empty!\";\n"
            "return false;\n"
          "}\n"
          "if (query.length > upperLimit) {\n"
            "document.getElementById(\"warning\").innerHTML = \"Query cannot exceed \" + upperLimit + \"!\";\n"
            "return false;\n"
          "}\n"
          "for (let i = 0; i < banned.length; i++) {\n"
            "if (query.indexOf(banned[i]) > -1) {\n"
              "document.getElementById(\"warning\").innerHTML = \"Do not type \" + banned[i];\n"
              "return false;\n"
            "}\n"
          "}\n"
          "return true;\n"
        "}\n"
      "</script>\n"
    "</html>";
  return body;
}