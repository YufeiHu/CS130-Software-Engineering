#include "gtest/gtest.h"
#include "request_handler_util.h"

TEST(RequestHandlerUtilTest, HandleRequestError) {
  std::unique_ptr<Reply> response_hat(new Reply());
  RequestHandlerUtil::HandleRequestError(*response_hat, 404, "Error");
  EXPECT_EQ(response_hat->status_code, 404);
  EXPECT_EQ(response_hat->mime_type, "html");
  EXPECT_EQ(response_hat->headers, "Content-Type: text/html\r\nContent-Length: 5\r\n");
  EXPECT_EQ(response_hat->content, "Error");
  response_hat.reset(nullptr);
}

TEST(RequestHandlerUtilTest, GetContentLengthTest) {
  std::string ans = "Content-Length: 5\r\n";
  EXPECT_EQ(RequestHandlerUtil::GetContentLength("Error"), ans);
}
