#include "gtest/gtest.h"
#include "meme_new_handler.h"

class MemeNewHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> meme_new_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  MemeNewHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    options->path_meme_request = {{"/meme_request", "/meme/create.html"}};
    meme_new_handler = MemeNewHandler::create(options);
  }

  ~MemeNewHandlerTest() {
    meme_new_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(MemeNewHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "POST /meme/request/m HTTP/1.1\r\n\r\n";
  request.method = "POST";
  request.url_full = "/meme/request/m";
  request.url_location = "/meme/request";
  request.url_filename = "m";
  request.url_extension = "";
  request.method = "HTTP/1.1";
  request.body = "image=index.jpg&top=1234&bottom=5678";

  response_hat = meme_new_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 404);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 29\r\n");
  // EXPECT_EQ(1, 1);
  // EXPECT_EQ(response_hat->content, "Created meme! Your meme has a ID as <a href=\"/meme/view/id="+"0"+"\">"+"0"+"</a>");
}
