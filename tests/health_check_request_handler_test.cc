#include "gtest/gtest.h"
#include "health_check_request_handler.h"

class HealthCheckRequestHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> health_check_request_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  HealthCheckRequestHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    health_check_request_handler = HealthCheckRequestHandler::create(options);
  }

  ~HealthCheckRequestHandlerTest() {
    health_check_request_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(HealthCheckRequestHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "GET /health_check_request HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/health_check_request";
  request.url_location = "";
  request.url_filename = "";
  request.url_extension = "";
  request.method = "HTTP/1.1";

  response_hat = health_check_request_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 200);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 6\r\n");
  EXPECT_EQ(response_hat->content, "200 OK");
}
