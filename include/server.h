//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVER
#define SERVER

#include <map>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "session.h"
#include "config_options.h"

using boost::asio::ip::tcp;

class Server {
 public:
  Server(boost::asio::io_service& io_service,
         ConfigOptions* options_upstream) : io_service_(io_service),
                                            acceptor_(io_service,
                                                      tcp::endpoint(tcp::v4(),
                                                                    options_upstream->port)) {
    options = options_upstream;
    StartAccept();
  }
  void run();

 private:
  ConfigOptions* options;
  tcp::acceptor acceptor_;
  boost::asio::io_service& io_service_;
  const int thread_num = 5;
  void StartAccept();
  void HandleAccept(Session* new_session,
                    const boost::system::error_code& error);
};

#endif
