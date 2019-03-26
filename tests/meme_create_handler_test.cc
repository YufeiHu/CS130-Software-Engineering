#include "gtest/gtest.h"
#include "meme_create_handler.h"

class MemeCreateHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> meme_create_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  MemeCreateHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    meme_create_handler = MemeCreateHandler::create(options);
  }

  ~MemeCreateHandlerTest() {
    meme_create_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(MemeCreateHandlerTest, HandleRequestTest) {
  Request request;
  // request.all = "GET /meme/create HTTP/1.1\r\n\r\n";
  // request.method = "GET";
  // request.url_full = "/";
  // request.url_location = "";
  // request.url_filename = "";
  // request.url_extension = "";
  // request.method = "HTTP/1.1";
  request.body = "image=index.jpg&top=1234&bottom=5678";

  response_hat = meme_create_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 400);
  EXPECT_EQ(response_hat->mime_type, "html");
  // EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 18\r\n");
  EXPECT_EQ(response_hat->content, "Error: request method is illegal!\r\n\r\n");
}
