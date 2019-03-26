#include "gtest/gtest.h"
#include "proxy_handler.h"

class ProxyHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> proxyHandler;
  std::unique_ptr<Reply> response;
  ConfigOptions* options;

  ProxyHandlerTest() {
    options = new ConfigOptions;
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    options->path_status = {{"/status", ""}};
    options->path_proxy = {{"/google", "www.google.com"}};
    proxyHandler = ProxyHandler::create(options);
  }

  ~ProxyHandlerTest() {
    proxyHandler.reset(nullptr);
    response.reset(nullptr);
    delete options;
  }
};

TEST_F(ProxyHandlerTest, HandlerSuccessfullyCreated) {
  EXPECT_NE(proxyHandler, nullptr);
}

TEST_F(ProxyHandlerTest, HandlerCanHandleRequest) {
  Request request;
  request.all = "GET /google HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/google";
  request.url_location = "/google";
  request.method = "HTTP/1.1";
  request.url_path = "/";
  request.url_root = "/google";

  response = proxyHandler->HandleRequest(request);
  EXPECT_NE(response->status_code, 404);
  EXPECT_NE(response->content, "");
}

TEST_F(ProxyHandlerTest, HandlerServesRelativeContent) {
  Request request;
  request.all = "GET /google/favicon.ico HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/google/favicon.ico";
  request.url_location = "/google";
  request.method = "HTTP/1.1";
  request.url_path = "/favicon.ico";
  request.url_root = "/google";

  response = proxyHandler->HandleRequest(request);
  EXPECT_NE(response->status_code, 404);
  EXPECT_NE(response->content, "");
}

TEST_F(ProxyHandlerTest, HandlerReturns500OnException) {
  Request request;
  request.all = "GET /google/favicon.ico HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/google/favicon.ico";
  request.url_location = "/google";
  request.method = "HTTP/1.1";
  request.url_root = "/google";

  response = proxyHandler->HandleRequest(request);
  EXPECT_NE(response->status_code, 500);
  EXPECT_NE(response->content, "Error servering proxy content!");
}
