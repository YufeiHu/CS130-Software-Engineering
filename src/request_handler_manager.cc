//
// Author: Yufei Hu, Zhechen Xu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "request_handler_manager.h"

std::unique_ptr<RequestHandler> RequestHandlerManager::createByName(const std::string& name,
                                                                    const ConfigOptions* options) {
  if (name.compare("echo") == 0)
    return EchoHandler::create(options);
  else if (name.compare("static") == 0)
    return FileHandler::create(options);
  else if (name.compare("404") == 0)
    return NotFoundHandler::create(options);
  else if (name.compare("400") == 0)
    return BadRequestHandler::create(options);
  else if (name.compare("status") == 0)
    return StatusHandler::create(options);
  else if (name.compare("meme_home") == 0)
    return MemeHomeHandler::create(options);
  else if (name.compare("meme_new") == 0)
    return MemeNewHandler::create(options);
  else if (name.compare("meme_create") == 0)
    return MemeCreateHandler::create(options);
  else if (name.compare("meme_view") == 0)
    return MemeViewHandler::create(options);
  else if (name.compare("meme_list") == 0)
    return MemeListHandler::create(options);
  else if (name.compare("meme_delete") == 0)
    return MemeDeleteHandler::create(options);
  else if (name.compare("proxy") == 0)
    return ProxyHandler::create(options);
  else if (name.compare("health_check_request") == 0)
    return HealthCheckRequestHandler::create(options);
  else
    return nullptr;
}
