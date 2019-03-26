//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "server.h"
#include "config_parser.h"
#include "config_options.h"

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void handler(const boost::system::error_code& error, int signal_number) {
  BOOST_LOG_TRIVIAL(fatal) << "main-> Ctrl-C signal captured, server shutdown";
  exit(signal_number);
}

void init_logging() {
  logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

  // store on logger file
  logging::add_file_log (
    keywords::file_name = "./logs/serverlog-%3N.txt",                             // name the files
    keywords::rotation_size = 10 * 1024 * 1024,                                   // size rotation by default 10M
    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), // daily updated at midnight
    keywords::format = "[%TimeStamp%] [%ThreadID%] [%ProcessID%] [%Severity%] %Message%",
    keywords::auto_flush = true
  );

  // enable log to be printed in console
  logging::add_console_log(std::cout, boost::log::keywords::format = ">>[%TimeStamp%] [%ThreadID%] [%ProcessID%] [%Severity%] %Message%"); 
  
  // set logging filter
  logging::core::get()->set_filter (
    logging::trivial::severity >= logging::trivial::info
  );
  logging::add_common_attributes();
}

int main(int argc, char* argv[]) {
  using namespace boost::log; 
  init_logging();
  boost::asio::io_service io_service;

  // construct a signal set registered for process termination
  boost::asio::signal_set signals(io_service, SIGINT);

  // start an asynchronous wait for one of the signals to occur
  signals.async_wait(handler);

  try {
    if (argc != 2) {
      std::cerr << "Usage: ./nuke_server <server_config_path>\n";
      BOOST_LOG_TRIVIAL(fatal) << "main-> server not started, missing config file";
      return 1;
    }

    NginxConfigParser ConfigParser;
    NginxConfig config;
    bool isParsed = ConfigParser.Parse(argv[1], &config);

    if (isParsed) {
      BOOST_LOG_TRIVIAL(info) << "main-> config = \n" << config.ToString();
    } else {
      BOOST_LOG_TRIVIAL(fatal) << "main-> error in parsing config file";
    }

    ConfigOptions* options = new ConfigOptions();
    config.GetConfigOptions(options);

    Server s(io_service, options);
    s.run();
    delete options;
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
    BOOST_LOG_TRIVIAL(error) << "main-> exception: " << e.what();
  }
  return 0;
}
