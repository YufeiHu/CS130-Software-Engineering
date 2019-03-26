//
// Author: Junheng Hao, Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "server_status.h"

void ServerStatus::LogRequest(std::string url, int status_code) {
  requests_logs.push_back(std::make_pair(url, status_code));
}

void ServerStatus::LogHandler(std::string prefix, std::string name) {
  for(int i = 0; i < handler_names.size(); i++) {
    if(handler_names[i].first == prefix) return;
  }
  handler_names.push_back(std::make_pair(prefix, name));
}

void ServerStatus::ResetStatus() {
  handler_names.clear();
  requests_logs.clear();
}

ServerStatus& ServerStatus::getInstance() {
  static ServerStatus instance;
  return instance;
}
