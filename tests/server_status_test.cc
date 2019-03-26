#include "gtest/gtest.h"
#include "server_status.h"

class ServerStatusTest : public ::testing::Test {
 protected:
  ServerStatus& server_status = ServerStatus::getInstance();
  std::vector<std::pair<std::string, int>> requests_logs;
  std::vector<std::pair<std::string, std::string>> handler_names;
};
    
TEST_F(ServerStatusTest, ResetStatusTest) {
  server_status.ResetStatus();
  EXPECT_EQ(server_status.GetNumRequest(), 0);
  EXPECT_EQ(server_status.GetNumHandler(), 0);
}

TEST_F(ServerStatusTest, LogRequestTest) {
  server_status.ResetStatus();
  server_status.LogRequest("GET /echo/ HTTP/1.1", 200);
  server_status.LogRequest("GET / HTTP/1.1", 404);
  EXPECT_EQ(server_status.GetNumRequest(), 2);
  requests_logs = server_status.getRequests();
  EXPECT_EQ(requests_logs[0].first, "GET /echo/ HTTP/1.1");
  EXPECT_EQ(requests_logs[1].first, "GET / HTTP/1.1");
  EXPECT_EQ(requests_logs[0].second, 200);
  EXPECT_EQ(requests_logs[1].second, 404);
}

TEST_F(ServerStatusTest, LogHandlerTest) {
  server_status.ResetStatus();
  server_status.LogHandler("/echo", "EchoHandler");
  server_status.LogHandler("/status", "StatusHandler");
  EXPECT_EQ(server_status.GetNumHandler(), 2);
  handler_names = server_status.getHandlers();
  EXPECT_EQ(handler_names[0].first, "/echo");
  EXPECT_EQ(handler_names[1].first, "/status");
  EXPECT_EQ(handler_names[0].second, "EchoHandler");
  EXPECT_EQ(handler_names[1].second, "StatusHandler");
}
