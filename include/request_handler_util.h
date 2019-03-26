//
// Author: Yufei Hu, Zhechen Xu, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef REQUEST_HANDLER_UTIL
#define REQUEST_HANDLER_UTIL

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

class Meme {
 public:
  int meme_id;
  std::string top_text;
  std::string bottom_text;
  std::string image_name;
  std::string image_url;
  Meme(std::string top_text_upstream,
       std::string bottom_text_upstream,
       std::string image_name_upstream,
       std::string image_url_upstream): top_text(top_text_upstream),
                                        bottom_text(bottom_text_upstream),
                                        image_name(image_name_upstream),
                                        image_url(image_url_upstream) { meme_id = -1; }
  Meme(int meme_id_upstream,
       std::string top_text_upstream,
       std::string bottom_text_upstream,
       std::string image_name_upstream,
       std::string image_url_upstream): meme_id(meme_id_upstream),
                                        top_text(top_text_upstream),
                                        bottom_text(bottom_text_upstream),
                                        image_name(image_name_upstream),
                                        image_url(image_url_upstream) { }
};

class Request {
 public:
  std::string all;
  std::string method;
  std::string url_full;
  std::string url_location;
  std::string url_filename;
  std::string url_extension;
  std::string url_root;
  std::string url_path;
  std::string protocol;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

class Reply {
 public:
  int status_code;
  std::string mime_type;
  std::string headers;
  std::string content;
  std::string GetResponse();
};

class RequestHandlerUtil {
 public:
  static std::unordered_map<std::string, std::string> ext_to_type;
  static std::unordered_map<int, std::string> status_code_to_line;
  static void HandleRequestError(Reply& http_response, int error_code, std::string error_body);
  static std::string GetContentLength(std::string response_body);
};

#endif
