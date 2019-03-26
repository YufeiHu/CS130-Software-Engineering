#include "gtest/gtest.h"
#include "bad_request_handler.h"

class BadRequestHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> bad_request_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  BadRequestHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    options->path_status = {{"/status", ""}};
    bad_request_handler = BadRequestHandler::create(options);
  }

  ~BadRequestHandlerTest() {
    bad_request_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(BadRequestHandlerTest, CreateTest) {
  EXPECT_NE(bad_request_handler, nullptr);
}

TEST_F(BadRequestHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "POST / HTTP/1.1\r\n\r\n";
  request.method = "POST";
  request.url_full = "/";
  request.url_location = "";
  request.url_filename = "";
  request.url_extension = "";
  request.method = "HTTP/1.1";

  response_hat = bad_request_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 400);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 23\r\n");
  EXPECT_EQ(response_hat->content, "Error: bad request!\r\n\r\n");
}