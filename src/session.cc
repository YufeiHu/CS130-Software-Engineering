//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "session.h"

tcp::socket& Session::Socket() {
  return socket_;
}

int64_t getCurrentTime() {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Session::Start(std::string ip_addr) {
  this->session_ip = ip_addr;
  DoRead();
}

std::string Session::GetIP() {
  return this->session_ip;
}

void Session::DoWrite(std::string to_send) {
  boost::asio::async_write(socket_,
    boost::asio::buffer(to_send.c_str(), to_send.size()),
      [this](boost::system::error_code ec, std::size_t) {
        if (!ec) {
          DoRead();
        }
      }
  );
}

bool Session::CheckInput(std::size_t length, char* buffer) {
  std::string received(buffer, buffer + length);
  msg += received;
  bool res = (msg.find("\r\n\r\n") != std::string::npos || msg.find("\n\n") != std::string::npos);
  return res;
}

void Session::ParseUrl(std::string url) {
  if (url.length() > 0 && url.at(url.length() - 1) == '/')
    http_request.url_full = url.substr(0, url.length() - 1);
  else
    http_request.url_full = url;

  for (int i = http_request.url_full.length() - 1; i >= 0; i--) {
    if (http_request.url_full.at(i) == '/') {
      if (i != 0)
        http_request.url_location = http_request.url_full.substr(0, i);
      http_request.url_filename = http_request.url_full.substr(i + 1, http_request.url_full.length());
      int length_filename = (int)http_request.url_filename.length();
      for (int j = 0; j < length_filename; j++) {
        if (j + 3 <= length_filename && http_request.url_filename.substr(j, 3).compare("?q=") == 0) {
          http_request.url_filename = http_request.url_filename.substr(0, j);
          break;
        }
        if (j + 4 <= length_filename && http_request.url_filename.substr(j, 4).compare("?id=") == 0) {
          http_request.url_filename = http_request.url_filename.substr(0, j);
          break;
        }
      }
      break;
    }
  }

  for (int i = http_request.url_filename.length() - 1; i >= 0; i--) {
    char c = http_request.url_filename.at(i);
    if (c == '.') {
      http_request.url_extension = http_request.url_filename.substr(i + 1, http_request.url_filename.length());
      break;
    }
  }

  for (int i = 0; i < http_request.url_full.length(); i++) {
    if (http_request.url_full.at(i) == '/' && i != 0) {
      http_request.url_root = http_request.url_full.substr(0, i);
      http_request.url_path = http_request.url_full.substr(i);
      break;
    }
  }

  if (http_request.url_root.empty()) {
    http_request.url_root = http_request.url_full;
  }

  if (http_request.url_path.empty()) {
    http_request.url_path = '/';
  }

  BOOST_LOG_TRIVIAL(info) << "Session::ParseUrl-> full = " << http_request.url_full;
  BOOST_LOG_TRIVIAL(info) << "Session::ParseUrl-> location = " << http_request.url_location;
  BOOST_LOG_TRIVIAL(info) << "Session::ParseUrl-> filename = " << http_request.url_filename;
  BOOST_LOG_TRIVIAL(info) << "Session::ParseUrl-> extension = " << http_request.url_extension;
  BOOST_LOG_TRIVIAL(info) << "Session::ParseUrl-> root = " << http_request.url_root;
  BOOST_LOG_TRIVIAL(info) << "Session::ParseUrl-> path = " << http_request.url_path;
}

short Session::ParseRequest(std::string request) {
  /*
  This function returns a status code and also passes different tokens
  of url to corresponding member variables
    0: legal
    1: not GET
    2: not HTTP/1.1
    3: incomplete
  */
  // split into line/headers and body
  bool body_found = false;
  std::string request_line_headers;
  int request_length = (int)request.length();
  for (int i = 0; i + 4 <= request_length; i++) {
    if (request.substr(i, 4).compare("\r\n\r\n") == 0) {
      request_line_headers = request.substr(0, i);
      http_request.body = request.substr(i + 4, request_length);
      body_found = true;
      break;
    }
  }
  if (body_found == false) {
    request_line_headers = request;
    http_request.body = "";
  }
  BOOST_LOG_TRIVIAL(info) << "Session::ParseRequest-> " \
                          << "request body = \n" << http_request.body;

  // split line/headers
  std::string request_line;
  std::vector<std::string> vec1;
  boost::split(vec1, request_line_headers, [](char c) {
    return (c == '\r' || c == '\n' || c == '\t');
  });
  std::vector<std::string> vec2;
  for (std::string token : vec1) {
    if (token.empty() == false)
      vec2.push_back(token);
  }
  if (vec2.size() == 0) {
    BOOST_LOG_TRIVIAL(error) << "Session::ParseRequest-> " \
                             << "[IP:" <<this->GetIP() <<"] " << "Parse request: " << "Incomplete";
    return 3;
  }
  for (int i = 1; i < vec2.size(); i++) {
    std::string header = vec2[i];
    int header_length = (int)header.length();
    for (int j = 0; j < header_length; j++) {
      if (header.at(j) == ':') {
        if (j + 1 <= header_length and header.at(j + 1) == ' ') {
          http_request.headers[header.substr(0, j)] = header.substr(j + 2, header_length);
        } else {
          http_request.headers[header.substr(0, j)] = header.substr(j + 1, header_length);
        }
        break;
      }
    }
  }
  request_line = vec2[0];
  for (auto it : http_request.headers) {
    BOOST_LOG_TRIVIAL(info) << "Session::ParseRequest-> " \
                            << "request header = " << it.first << ": " << it.second;
  }
  BOOST_LOG_TRIVIAL(info) << "Session::ParseRequest-> " \
                          << "request line = " << request_line;

  // split request line
  std::vector<std::string> vec3;
  boost::split(vec3, request_line, [](char c) {
    return (c == ' ' || c == '\r' || c == '\n' || c == '\t');
  });
  std::vector<std::string> vec4;
  for (std::string token : vec3) {
    if (token.empty() == false)
      vec4.push_back(token);
  }
  int length_vec4 = vec4.size();
  if (length_vec4 < 3) {
    BOOST_LOG_TRIVIAL(error) << "Session::ParseRequest-> " \
                             << "[IP:" <<this->GetIP() <<"] " << "Parse request: " << "Incomplete";
    return 3;
  }
  if (vec4[2].compare("HTTP/1.1") != 0) {
    BOOST_LOG_TRIVIAL(error) << "Session::ParseRequest-> " \
                             << "[IP:" <<this->GetIP() <<"] " << "Parse request: " << "Not HTTP/1.1";
    return 2;
  }
  http_request.all = request;
  http_request.method = vec4[0];
  http_request.protocol = vec4[2];
  ParseUrl(vec4[1]);
  BOOST_LOG_TRIVIAL(info) << "Session::ParseRequest-> " \
                          << "[IP:" <<this->GetIP() <<"] " << "Parse request: " << "Legal";
  return 0;
}

std::string Session::HandlerDispatcher() {
  short request_status = ParseRequest(msg);
  if (request_status == 0) {
    if (http_request.url_full.length() <= 1) {
      BOOST_LOG_TRIVIAL(error) << "Session::HandlerDispatcher->" \
                               << "ERROR(404): URL is empty, ::matched:: NotFoundHandler";
      return "404";
    } else if (options->path_echo.find(http_request.url_root) != options->path_echo.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: EchoHandler";
      return "echo";
    } else if (options->path_health_check_request.find(http_request.url_root) != options->path_health_check_request.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: HealthCheckRequestHandler";
      return "health_check_request";
    } else if (options->path_status.find(http_request.url_full) != options->path_status.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: StatusHandler";
      return "status";
    } else if (options->path_file.find(http_request.url_location) != options->path_file.end()) {
      if (http_request.url_filename.length() == 0 || http_request.url_extension.length() == 0) {
        BOOST_LOG_TRIVIAL(error) << "Session::HandlerDispatcher->" \
                                 << "ERROR(404): URL path or extension is illegal, ::matched:: NotFoundHandler";
        return "404";
      } else {
        BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                                << "successfully ::matched:: FileHandler";
        return "static";
      }
    } else if (options->path_meme_home.find(http_request.url_full) != options->path_meme_home.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: MemeHomeHandler";
      return "meme_home";
    } else if (options->path_meme_new.find(http_request.url_location + '/' + http_request.url_filename) != options->path_meme_new.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: MemeNewHandler";
      return "meme_new";
    } else if (options->path_meme_create.find(http_request.url_location + '/' + http_request.url_filename) != options->path_meme_create.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: MemeCreateHandler";
      return "meme_create";
    } else if (options->path_meme_view.find(http_request.url_location) != options->path_meme_view.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: MemeViewHandler";
      return "meme_view";
    } else if (options->path_meme_delete.find(http_request.url_location) != options->path_meme_delete.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: MemeDeleteHandler";
      return "meme_delete";
    } else if (options->path_meme_list.find(http_request.url_location + '/' + http_request.url_filename) != options->path_meme_list.end()) {
      BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                              << "successfully ::matched:: MemeListHandler";
      return "meme_list";
    } else if (options->path_proxy.find(http_request.url_root) != options->path_proxy.end()) {
        BOOST_LOG_TRIVIAL(info) << "Session::HandlerDispatcher-> " \
                                << "successfully ::matched:: ProxyHandler";
        return "proxy";
    } else {
      BOOST_LOG_TRIVIAL(error) << "Session::HandlerDispatcher->" \
                               << "ERROR(404): URL type is not supported, ::matched:: NotFoundHandler";
      return "404";
    }
  } else if (request_status == 1) {
    BOOST_LOG_TRIVIAL(error) << "Session::HandlerDispatcher->" \
                             << "ERROR(400): HTTP method is not GET, ::matched:: BadRequestHandler";
    return "400";
  } else if (request_status == 2) {
    BOOST_LOG_TRIVIAL(error) << "Session::HandlerDispatcher->" \
                             << "ERROR(400): HTTP protocol is not HTTP/1.1, ::matched:: BadRequestHandler";
    return "400";
  } else {
    BOOST_LOG_TRIVIAL(error) << "Session::HandlerDispatcher->" \
                             << "ERROR(400): HTTP request line is incomplete, ::matched:: BadRequestHandler";
    return "400";
  }
}

void Session::DoRead() {
  socket_.async_read_some(boost::asio::buffer(data_, max_length),
    [this](boost::system::error_code ec, std::size_t length) {
      bool reached_end = CheckInput(length, data_);
      if (!ec) {
        if (reached_end) {
          std::string handler_name = HandlerDispatcher();
          std::unique_ptr<RequestHandler> request_handler = RequestHandlerManager::createByName(handler_name, options);
          std::unique_ptr<Reply> http_response = request_handler->HandleRequest(http_request);
          BOOST_LOG_TRIVIAL(info) << "Session::DoRead-> " \
                                  << "Status Recorded:[" << http_request.all << "] Response:[" << http_response->status_code<<"]";
          ServerStatus::getInstance().LogRequest(http_request.all, http_response->status_code);
          DoWrite(http_response->GetResponse());
          http_response.reset(nullptr);
          msg = "";
          http_request.all = "";
          http_request.method = "";
          http_request.url_full = "";
          http_request.url_location = "";
          http_request.url_filename = "";
          http_request.url_extension = "";
          http_request.url_root = "";
          http_request.url_path = "";
          http_request.protocol = "";
        } else {
          DoRead();
        }
      }
    }
  );
}
