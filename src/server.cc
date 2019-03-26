//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.h"

void Server::StartAccept() {
  Session* new_session = new Session(io_service_, options);
  acceptor_.async_accept(new_session->Socket(),
  											 boost::bind(&Server::HandleAccept, this, new_session,
                                     boost::asio::placeholders::error));
}

void Server::HandleAccept(Session* new_session, const boost::system::error_code& error) {
  std::string ip_addr = new_session->Socket().remote_endpoint().address().to_string();
  BOOST_LOG_TRIVIAL(info) << "Server::HandleAccept-> " \
                          << "connected to new clients from IP:" << ip_addr;
  if (!error) {
    new_session->Start(ip_addr);
  } else {
    delete new_session;
  }
  StartAccept();
}

void Server::run(){
  std::vector<boost::shared_ptr<boost::thread>> thread_pool;
  for(int i=0; i < thread_num; i++){
    boost::shared_ptr<boost::thread> thread(new 
      boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
    thread_pool.push_back(thread);
  }
  for (int i = 0; i < thread_pool.size(); i++) {
    thread_pool[i]->join();
  }
}
