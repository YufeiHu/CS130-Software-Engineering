//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SESSION
#define SESSION

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "server_status.h"
#include "config_options.h"
#include "request_handler.h"
#include "request_handler_util.h"
#include "request_handler_manager.h"

using boost::asio::ip::tcp;

class Session {
 public:
  Session(boost::asio::io_service& io_service,
          ConfigOptions* options_upstream) : socket_(io_service),
                                             options(options_upstream) { }
  Request http_request;
  const ConfigOptions* options;
  std::string msg;
  tcp::socket& Socket();
  std::string GetIP();
  void Start(std::string ip_addr);
  bool CheckInput(std::size_t length, char* buffer);
  void ParseUrl(std::string url);
  short ParseRequest(std::string request);
  std::string HandlerDispatcher();

 private:
  tcp::socket socket_;
  std::string session_ip = "127.0.0.1";
  enum { max_length = 2048 };
  char data_[max_length];
  void DoWrite(std::string to_send);
  void DoRead();
};

#endif
