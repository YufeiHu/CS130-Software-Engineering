#include "gtest/gtest.h"
#include "session.h"

class SessionTest : public ::testing::Test {
 protected:
  ConfigOptions* options;
  boost::asio::io_service io_service;
  Session* s;

  SessionTest() {
    options = new ConfigOptions();
    options->port = 8080;
    options->root = "../static";
    options->path_echo = {{"/echo", ""}, {"/print", ""}};
    options->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    options->path_status = {{"/status", ""}};
    options->path_proxy = {{ "/reverse", "www.google.com"}};
    s = new Session(io_service, options);
  }

  bool check_input_test(std::string str) {
    char buffer[256];
    std::size_t size = str.size();
    strncpy(buffer, str.c_str(), size);
    return s->CheckInput(size, buffer);
  }

  void parse_url(std::string str) {
    s->ParseUrl(str);
  }

  short parse_request(std::string request) {
    return s->ParseRequest(request);
  }

  std::string handler_dispatcher(std::string request) {
    s->msg = request;
    return s->HandlerDispatcher();
  }

  void http_request_clear() {
    s->http_request.all = "";
    s->http_request.method = "";
    s->http_request.url_full = "";
    s->http_request.url_location = "";
    s->http_request.url_filename = "";
    s->http_request.url_extension = "";
    s->http_request.url_root = "";
    s->http_request.url_path = "";
    s->http_request.protocol = "";
  }

  ~SessionTest() {
    delete options;
    delete s;
  }
};

TEST_F(SessionTest, CheckInputTest) {
  EXPECT_TRUE(check_input_test("GET / HTTP/1.1\r\n\r\n"));
}

TEST_F(SessionTest, ParseUrlTest) {
  parse_url("/img/index.html");
  EXPECT_EQ(s->http_request.url_full, "/img/index.html");
  EXPECT_EQ(s->http_request.url_location, "/img");
  EXPECT_EQ(s->http_request.url_filename, "index.html");
  EXPECT_EQ(s->http_request.url_extension, "html");
}

TEST_F(SessionTest, ParseRequestTest) {
  EXPECT_EQ(parse_request("GET / HTTP/1.1\r\n\r\n"), 0);
  EXPECT_EQ(parse_request("GET / HTTP/2.0\r\n\r\n"), 2);
  EXPECT_EQ(parse_request("GET /\r\n\r\n"), 3);
  EXPECT_EQ(parse_request("POST /meme/create HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ntop=Horror&bottom=Movie\r\n\r\n"), 0);
  EXPECT_EQ(parse_request(""), 3);
}

TEST_F(SessionTest, GetResponseTest) {
  EXPECT_EQ(handler_dispatcher("GET /\r\n\r\n"), "400");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET / HTTP/2.0\r\n\r\n"), "400");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET / HTTP/1.1\r\n\r\n"), "404");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET /echo HTTP/1.1\r\n\r\n"), "echo");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET /echo/hello HTTP/1.1\r\n\r\n"), "echo");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET /status/message HTTP/1.1\r\n\r\n"), "404");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET /img/abc HTTP/1.1\r\n\r\n"), "404");
  http_request_clear();
  EXPECT_EQ(handler_dispatcher("GET /img/index.jpg HTTP/1.1\r\n\r\n"), "static");
  http_request_clear();
}
