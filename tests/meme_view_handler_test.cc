#include "gtest/gtest.h"
#include "meme_view_handler.h"

class MemeViewHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> meme_view_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  MemeViewHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    meme_view_handler = MemeViewHandler::create(options);
  }

  ~MemeViewHandlerTest() {
    meme_view_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(MemeViewHandlerTest, HandleRequestTest) {
  Request request;
  request.all = "GET /meme/view/id=300 HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/meme/view/id=300";
  request.url_location = "";
  request.url_filename = "";
  request.url_extension = "";
  request.method = "HTTP/1.1";
  // request.body = "image=index.jpg&top=1234&bottom=5678";

  response_hat = meme_view_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 404);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 36\r\n");
  // EXPECT_EQ(response_hat->content, "Created meme! Your meme has a ID as <a href=\"/meme/view/id="+"0"+"\">"+"0"+"</a>");
}
