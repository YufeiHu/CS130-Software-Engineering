#include "gtest/gtest.h"
#include "status_handler.h"

class StatusHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> status_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  StatusHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    status_handler = StatusHandler::create(options);
  }

  ~StatusHandlerTest() {
    status_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(StatusHandlerTest, CreateTest) {
  EXPECT_NE(status_handler, nullptr);
}

TEST_F(StatusHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "GET /status/messgae HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/status/messgae";
  request.url_location = "";
  request.url_filename = "";
  request.url_extension = "";
  request.method = "HTTP/1.1";
  
  ServerStatus::getInstance().ResetStatus();
  response_hat = status_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 200);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 301\r\n");
  EXPECT_EQ(response_hat->content, "<html><head><title>Server Status</title></head><h2>Requests</h2><p> Total Number of Requests: 0</p><table><tr><th>URL Requested</th><th>Response Code</th></tr></table><br><h2>Handlers</h2><p> Total Number of Handlers: 0</p><table><tr><th>URI Prefix</th><th>Request Handler</th></tr></table><br></html>");
}
