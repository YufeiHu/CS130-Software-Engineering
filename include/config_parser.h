//
// Author: Zhechen Xu, Junheng Hao
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef CONFIG_PARSER
#define CONFIG_PARSER

#include <stack>
#include <memory>
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "server_status.h"
#include "config_options.h"

class NginxConfig;

// the parsed representation of a single config statement
class NginxConfigStatement {
 public:
  std::string ToString(int depth);
  std::vector<std::string> tokens_;
  std::unique_ptr<NginxConfig> child_block_;
  void GetKeyValue(ConfigOptions* options, std::string* request_type);
};

// the parsed representation of the entire config
class NginxConfig {
 public:
  std::string ToString(int depth = 0);
  std::vector<std::shared_ptr<NginxConfigStatement>> statements_;
  void GetConfigOptions(ConfigOptions* options);
};

// the driver that parses a config file and generates an NginxConfig
class NginxConfigParser {
 public:
  NginxConfigParser() {}
  bool Parse(std::istream* config_file, NginxConfig* config);
  bool Parse(const char* file_name, NginxConfig* config);

 private:
  enum TokenType {
    TOKEN_TYPE_START = 0,
    TOKEN_TYPE_NORMAL = 1,
    TOKEN_TYPE_START_BLOCK = 2,
    TOKEN_TYPE_END_BLOCK = 3,
    TOKEN_TYPE_COMMENT = 4,
    TOKEN_TYPE_STATEMENT_END = 5,
    TOKEN_TYPE_EOF = 6,
    TOKEN_TYPE_ERROR = 7
  };
  const char* TokenTypeAsString(TokenType type);
  enum TokenParserState {
    TOKEN_STATE_INITIAL_WHITESPACE = 0,
    TOKEN_STATE_SINGLE_QUOTE = 1,
    TOKEN_STATE_DOUBLE_QUOTE = 2,
    TOKEN_STATE_TOKEN_TYPE_COMMENT = 3,
    TOKEN_STATE_TOKEN_TYPE_NORMAL = 4
  };
  TokenType ParseToken(std::istream* input, std::string* value, std::stack<char>& quote_stack);
};

#endif
