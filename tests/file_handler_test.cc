#include "gtest/gtest.h"
#include "file_handler.h"

class FileHandlerTest : public ::testing::Test {
 protected:
  std::unique_ptr<RequestHandler> file_handler;
  std::unique_ptr<Reply> response_hat;
  ConfigOptions* options;
  
  FileHandlerTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    file_handler = FileHandler::create(options);
  }

  ~FileHandlerTest() {
    file_handler.reset(nullptr);
    response_hat.reset(nullptr);
    delete options;
  }
};

TEST_F(FileHandlerTest, LegalHandleRequestTest1) {
  Request request;
  request.all = "GET /text/index.txt HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/text/index.txt";
  request.url_location = "/text";
  request.url_filename = "index.txt";
  request.url_extension = "txt";
  request.method = "HTTP/1.1";

  response_hat = file_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 200);
  EXPECT_EQ(response_hat->mime_type, "txt");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/plain\r\nContent-Length: 11\r\n");
  EXPECT_EQ(response_hat->content, "Nuke Server");
}

TEST_F(FileHandlerTest, LegalHandleRequestTest2) {
  Request request;
  request.all = "GET /text/index.md HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/text/index.md";
  request.url_location = "/text";
  request.url_filename = "index.md";
  request.url_extension = "md";
  request.method = "HTTP/1.1";

  response_hat = file_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 200);
  EXPECT_EQ(response_hat->mime_type, "md");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 21\r\n");
  EXPECT_EQ(response_hat->content, "<h1>Nuke Server</h1>\n");
}

TEST_F(FileHandlerTest, IllegalHandleRequestTest) {
  Request request;
  request.all = "GET /img/home.png HTTP/1.1\r\n\r\n";
  request.method = "GET";
  request.url_full = "/img/home.png";
  request.url_location = "/img";
  request.url_filename = "home.png";
  request.url_extension = "png";
  request.method = "HTTP/1.1";

  response_hat = file_handler->HandleRequest(request);
  EXPECT_EQ(response_hat->status_code, 404);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 49\r\n");
  EXPECT_EQ(response_hat->content, "Error: cannot open the file in the given URL!\r\n\r\n");
}
