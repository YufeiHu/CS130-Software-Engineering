#include "gtest/gtest.h"
#include "echo_handler.h"

class EchoHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> echo_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  EchoHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    echo_handler = EchoHandler::create(options);
  }

  ~EchoHandlerTest() {
    echo_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(EchoHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "GET / HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/";
  request.url_location = "";
  request.url_filename = "";
  request.url_extension = "";
  request.method = "HTTP/1.1";

  response_hat = echo_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 200);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 18\r\n");
  EXPECT_EQ(response_hat->content, "GET / HTTP/1.1\r\n\r\n");
}
