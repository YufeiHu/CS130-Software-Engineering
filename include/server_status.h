//
// Author: Junheng Hao, Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef SERVER_STATUS
#define SERVER_STATUS

#include <vector>
#include <utility>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "status_handler.h"

class ServerStatus{
 public:
  static ServerStatus& getInstance();
  void LogRequest(std::string url, int status_code);
  void LogHandler(std::string prefix, std::string name);
  void ResetStatus();
  int GetNumRequest() { return requests_logs.size(); }
  int GetNumHandler() { return handler_names.size(); }
  std::vector<std::pair<std::string, std::string>> getHandlers() { return handler_names; }
  std::vector<std::pair<std::string, int>> getRequests() { return requests_logs; }
  
 private:
  ServerStatus() {} 
  std::vector<std::pair<std::string, int>> requests_logs;
  std::vector<std::pair<std::string, std::string>> handler_names;
};

#endif
