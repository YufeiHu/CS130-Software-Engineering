// An nginx config file parser.
//
// See:
//   http://wiki.nginx.org/Configuration
//   http://blog.martinfjordvald.com/2010/07/nginx-primer/
//
// How Nginx does it:
//   http://lxr.nginx.org/source/src/core/ngx_conf_file.c

#include "config_parser.h"

std::string NginxConfig::ToString(int depth) {
  std::string serialized_config;
  for (const auto& statement : statements_) {
    serialized_config.append(statement->ToString(depth));
  }
  return serialized_config;
}

void NginxConfig::GetConfigOptions(ConfigOptions* options) {
  for (unsigned i = 0; i < statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> config_statement = statements_[i];
    if (config_statement->tokens_.size() == 2 && config_statement->tokens_[0] == "port") {
      options->port = stoi(config_statement->tokens_[1]);
    } else if (config_statement->tokens_.size() == 2 && config_statement->tokens_[0] == "root") {
      options->root = config_statement->tokens_[1];
    } else if (config_statement->tokens_.size() == 2 && config_statement->tokens_[0] == "handler") {
      config_statement->GetKeyValue(options, &config_statement->tokens_[1]);
    }
  }
}

void NginxConfigStatement::GetKeyValue(ConfigOptions* options, std::string* request_type) {
  if (child_block_.get() != nullptr) {
    std::string temp_key = "";
    for (unsigned i = 0; i < child_block_->statements_.size(); i++) {
      std::shared_ptr<NginxConfigStatement> config_statement = child_block_->statements_[i];
      if (config_statement->tokens_.size() == 2 && config_statement->tokens_[0] == "location") {
        if (*request_type == "static") {
          options->path_file.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "FileHandler");
          temp_key = config_statement->tokens_[1];
        } else if (*request_type == "echo") {
          options->path_echo.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "EchoHandler");
        } else if (*request_type == "health_check_request") {
          options->path_health_check_request.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "HealthCheckRequestHandler");
        } else if (*request_type == "status") {
          options->path_status.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "StatusHandler");
        } else if (*request_type == "meme_home") {
          options->path_meme_home.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "MemeHomeHandler"); 
          temp_key = config_statement->tokens_[1];
        } else if (*request_type == "meme_new") {
          options->path_meme_new.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "MemeNewHandler"); 
          temp_key = config_statement->tokens_[1];
        } else if (*request_type == "meme_create") {
          options->path_meme_create.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "MemeCreateHandler"); 
        } else if (*request_type == "meme_view") {
          options->path_meme_view.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "MemeViewHandler"); 
        } else if (*request_type == "meme_delete") {
          options->path_meme_delete.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "MemeDeleteHandler"); 
        } else if (*request_type == "meme_list") {
          options->path_meme_list.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "MemeListHandler"); 
        } else if (*request_type == "proxy") {
          options->path_proxy.insert({config_statement->tokens_[1], ""});
          ServerStatus::getInstance().LogHandler(config_statement->tokens_[1], "ProxyHandler");
          temp_key = config_statement->tokens_[1];
        }
      } else if (config_statement->tokens_.size() == 2 && config_statement->tokens_[0] == "root") {
        auto if_find = options->path_file.find(temp_key);
        if (if_find != options->path_file.end()) {
          if_find->second = config_statement->tokens_[1];
        }
        auto if_find2 = options->path_meme_new.find(temp_key);
        if (if_find2 != options->path_meme_new.end()) {
          if_find2->second = config_statement->tokens_[1];
        }
        auto if_find3 = options->path_meme_home.find(temp_key);
        if (if_find3 != options->path_meme_home.end()) {
          if_find3->second = config_statement->tokens_[1];
        }
      } else if (config_statement->tokens_.size() == 2 && config_statement->tokens_[0] == "destination") {
        auto if_find = options->path_proxy.find(temp_key);
        if (if_find != options->path_proxy.end()) {
          if_find->second = config_statement->tokens_[1];
        }
      }
    }
  }
  ServerStatus::getInstance().LogHandler("/", "NotFoundHandler");
}

std::string NginxConfigStatement::ToString(int depth) {
  std::string serialized_statement;
  for (int i = 0; i < depth; ++i) {
    serialized_statement.append("  ");
  }
  for (unsigned int i = 0; i < tokens_.size(); ++i) {
    if (i != 0) {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }
  if (child_block_.get() != nullptr) {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->ToString(depth + 1));
    for (int i = 0; i < depth; ++i) {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  } else {
    serialized_statement.append(";");
  }
  serialized_statement.append("\n");
  return serialized_statement;
}

const char* NginxConfigParser::TokenTypeAsString(TokenType type) {
  switch (type) {
    case TOKEN_TYPE_START:         return "TOKEN_TYPE_START";
    case TOKEN_TYPE_NORMAL:        return "TOKEN_TYPE_NORMAL";
    case TOKEN_TYPE_START_BLOCK:   return "TOKEN_TYPE_START_BLOCK";
    case TOKEN_TYPE_END_BLOCK:     return "TOKEN_TYPE_END_BLOCK";
    case TOKEN_TYPE_COMMENT:       return "TOKEN_TYPE_COMMENT";
    case TOKEN_TYPE_STATEMENT_END: return "TOKEN_TYPE_STATEMENT_END";
    case TOKEN_TYPE_EOF:           return "TOKEN_TYPE_EOF";
    case TOKEN_TYPE_ERROR:         return "TOKEN_TYPE_ERROR";
    default:                       return "Unknown token type";
  }
}

void update_quote_stack(std::stack<char>& quote_stack, char c) {
  if (c == '"' || c == '\'') {
    if (quote_stack.empty()) {
      quote_stack.push(c);
    } else {
      char c_prev = quote_stack.top();
      if (c == c_prev) {
        quote_stack.pop();
      } else {
        quote_stack.push(c);
      }
    }
  }
}

NginxConfigParser::TokenType NginxConfigParser::ParseToken(std::istream* input,
                                                           std::string* value,
                                                           std::stack<char>& quote_stack) {
  TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
  while (input->good()) {
    const char c = input->get();
    update_quote_stack(quote_stack, c);
    if (!input->good()) {
      break;
    }
    switch (state) {
      case TOKEN_STATE_INITIAL_WHITESPACE:
        switch (c) {
          case '{':
            *value = c;
            return TOKEN_TYPE_START_BLOCK;
          case '}':
            *value = c;
            return TOKEN_TYPE_END_BLOCK;
          case '#':
            *value = c;
            state = TOKEN_STATE_TOKEN_TYPE_COMMENT;
            continue;
          case '"':
            *value = c;
            state = TOKEN_STATE_DOUBLE_QUOTE;
            continue;
          case '\'':
            *value = c;
            state = TOKEN_STATE_SINGLE_QUOTE;
            continue;
          case ';':
            *value = c;
            return TOKEN_TYPE_STATEMENT_END;
          case ' ':
          case '\t':
          case '\n':
          case '\r':
            continue;
          default:
            *value += c;
            state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
            continue;
        }
      case TOKEN_STATE_SINGLE_QUOTE:
        // TODO: the end of a quoted token should be followed by whitespace.
        // TODO: Maybe also define a QUOTED_STRING token type.
        // TODO: Allow for backslash-escaping within strings.
        *value += c;
        if (c == '\'') {
          return TOKEN_TYPE_NORMAL;
        }
        continue;
      case TOKEN_STATE_DOUBLE_QUOTE:
        *value += c;
        if (c == '"') {
          return TOKEN_TYPE_NORMAL;
        }
        continue;
      case TOKEN_STATE_TOKEN_TYPE_COMMENT:
        if (c == '\n' || c == '\r') {
          return TOKEN_TYPE_COMMENT;
        }
        *value += c;
        continue;
      case TOKEN_STATE_TOKEN_TYPE_NORMAL:
        if (c == ' ' || c == '\t' || c == '\n' || c == '\t' ||
            c == ';' || c == '{' || c == '}') {
          input->unget();
          return TOKEN_TYPE_NORMAL;
        }
        *value += c;
        continue;
    }
  }

  if (state == TOKEN_STATE_SINGLE_QUOTE ||
      state == TOKEN_STATE_DOUBLE_QUOTE) {
    return TOKEN_TYPE_ERROR;
  }

  return TOKEN_TYPE_EOF;
}

bool NginxConfigParser::Parse(std::istream* config_file, NginxConfig* config) {
  std::stack<char> quote_stack;
  std::stack<NginxConfig*> config_stack;
  config_stack.push(config);
  TokenType last_token_type = TOKEN_TYPE_START;
  TokenType token_type;

  // Check if a parameter is fed into the file after its name
  int parameter_flag = 0;
  // Check if blocks are valid
  int block_match_flag = 0;

  while (true) {
    std::string token;
    token_type = ParseToken(config_file, &token, quote_stack);
    // printf ("%s: %s\n", TokenTypeAsString(token_type), token.c_str());
    if (token_type == TOKEN_TYPE_ERROR) {
      break;
    }

    if (token_type == TOKEN_TYPE_COMMENT) {
      // Skip comments.
      continue;
    }

    if (token_type == TOKEN_TYPE_START) {
      // Error.
      break;
    } else if (token_type == TOKEN_TYPE_NORMAL) {
      if (last_token_type == TOKEN_TYPE_START ||
          last_token_type == TOKEN_TYPE_STATEMENT_END ||
          last_token_type == TOKEN_TYPE_START_BLOCK ||
          last_token_type == TOKEN_TYPE_END_BLOCK ||
          last_token_type == TOKEN_TYPE_NORMAL) {
        if (last_token_type != TOKEN_TYPE_NORMAL) {
          config_stack.top()->statements_.emplace_back(new NginxConfigStatement);
        }
        config_stack.top()->statements_.back().get()->tokens_.push_back(token);
      } else {
        // Error.
        break;
      }
      // Set parameter flag
      parameter_flag += 1;
    } else if (token_type == TOKEN_TYPE_STATEMENT_END) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
      // Check parameter flag
      if (parameter_flag < 2) {
        break;
      } else {
        parameter_flag = 0;
      }

    } else if (token_type == TOKEN_TYPE_START_BLOCK) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
      NginxConfig* const new_config = new NginxConfig;
      config_stack.top()->statements_.back().get()->child_block_.reset(new_config);
      config_stack.push(new_config);
      // Reset parameter flag
      parameter_flag = 0;
      // Left
      block_match_flag += 1;
    } else if (token_type == TOKEN_TYPE_END_BLOCK) {
      if (last_token_type != TOKEN_TYPE_STATEMENT_END) {
        // Error.
        break;
      }
      config_stack.pop();
      // Reset parameter flag
      parameter_flag = 0;
      // Right
      block_match_flag -= 1;
      // Right more than left
      if (block_match_flag < 0) {
        break;
      }
    } else if (token_type == TOKEN_TYPE_EOF) {
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK) {
        // Error.
        break;
      }
      // Dont match
      if (block_match_flag != 0) {
        break;
      }
      if (quote_stack.empty() == false) {
        break;
      }
      return true;
    } else {
      // Error. Unknown token.
      break;
    }
    last_token_type = token_type;
  }

  printf("Bad transition from %s to %s\n",
         TokenTypeAsString(last_token_type),
         TokenTypeAsString(token_type));
  BOOST_LOG_TRIVIAL(error) << "NginxConfigParser::Parse-> bad transition from " \
                           << TokenTypeAsString(last_token_type) << " to " \
                           << TokenTypeAsString(token_type);
  return false;
}

bool NginxConfigParser::Parse(const char* file_name, NginxConfig* config) {
  std::ifstream config_file;
  config_file.open(file_name);
  if (!config_file.good()) {
    printf ("Failed to open config file: %s\n", file_name);
    BOOST_LOG_TRIVIAL(error) << "NginxConfigParser::Parse-> failed to open config file: " \
                             << file_name;
    return false;
  }
  const bool return_value = Parse(dynamic_cast<std::istream*>(&config_file), config);
  config_file.close();
  return return_value;
}
