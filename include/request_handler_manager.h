//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef REQUEST_HANDLER_MANAGER
#define REQUEST_HANDLER_MANAGER

#include <string>
#include <cstdlib>
#include <iostream>

#include "echo_handler.h"
#include "file_handler.h"
#include "proxy_handler.h"
#include "config_options.h"
#include "status_handler.h"
#include "request_handler.h"
#include "not_found_handler.h"
#include "bad_request_handler.h"

#include "meme_home_handler.h"
#include "meme_list_handler.h"
#include "meme_create_handler.h"
#include "meme_view_handler.h"
#include "meme_new_handler.h"
#include "meme_delete_handler.h"

#include "health_check_request_handler.h"

class RequestHandlerManager {
 public:
  static std::unique_ptr<RequestHandler> createByName(const std::string& name,
                                                      const ConfigOptions* options);
};

#endif
