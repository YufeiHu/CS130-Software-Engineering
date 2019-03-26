#include "gtest/gtest.h"
#include "not_found_handler.h"

class NotFoundHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> not_found_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  NotFoundHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    options->path_status = {{"/status", ""}};
    not_found_handler = NotFoundHandler::create(options);
  }

  ~NotFoundHandlerTest() {
    not_found_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(NotFoundHandlerTest, CreateTest) {
  EXPECT_NE(not_found_handler, nullptr);
}

TEST_F(NotFoundHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "GET /text/home1 HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/";
  request.url_location = "";
  request.url_filename = "";
  request.url_extension = "";
  request.method = "HTTP/1.1";

  response_hat = not_found_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 404);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 48\r\n");
  EXPECT_EQ(response_hat->content, "<html><body><h1>Not Found</h1></body></html>\r\n\r\n");
}
