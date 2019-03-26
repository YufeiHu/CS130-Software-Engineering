//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef CONFIG_OPTIONS
#define CONFIG_OPTIONS

#include <string>
#include <unordered_map>

struct ConfigOptions {
  short port;
  std::string root;
  std::unordered_map<std::string, std::string> path_echo;                  // {{"/echo", ""}}
  std::unordered_map<std::string, std::string> path_file;                  // {{"/img", "/img/"}}
  std::unordered_map<std::string, std::string> path_status;                // {{"/status", ""}}
  std::unordered_map<std::string, std::string> path_proxy;                 // {{"/google", "www.google.com"}}
  std::unordered_map<std::string, std::string> path_meme_home;             // {{"/meme/home", "/meme/home.html"}}
  std::unordered_map<std::string, std::string> path_meme_new;              // {{"/meme/new", "/meme/new.html"}}
  std::unordered_map<std::string, std::string> path_meme_create;           // {{"/meme/create", ""}}
  std::unordered_map<std::string, std::string> path_meme_view;             // {{"/meme/view", ""}}
  std::unordered_map<std::string, std::string> path_meme_list;             // {{"/meme/list", ""}}
  std::unordered_map<std::string, std::string> path_meme_delete;           // {{"/meme/delete", ""}}
  std::unordered_map<std::string, std::string> path_health_check_request;  // {{"/health", ""}}
};

#endif
