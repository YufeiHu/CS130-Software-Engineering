#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserTest : public ::testing::Test {
 protected:
  NginxConfigParser parser;
  NginxConfig out_config;
  ConfigOptions* options_true;

  NginxConfigParserTest() {
    options_true = new ConfigOptions();
    options_true->port = 8080;
    options_true->root = "../static";
    options_true->path_echo = {{"/echo", ""}, {"/print", ""}};
    options_true->path_file = {{"/img", "/img/"}, {"/app", "/app/"}, {"/text", "/text/"}};
    options_true->path_status = {{"/status", ""}};
  }

  ~NginxConfigParserTest() {
    delete options_true;
  }
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser.Parse("config_simple", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, SimpleConfig_parameter) {
  bool success = parser.Parse("config_parameter", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, SimpleConfig_block) {
  bool success = parser.Parse("config_block", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, ComplexConfig) {
  bool success = parser.Parse("config_complex", &out_config);
  std::string serialized_config = out_config.ToString();
  std::string target_config = "foo \"bar\";\nserver {\n  listen 80;\n"
                              "  client {\n    length 10;\n  }\n  ser"
                              "ver_name foo.com;\n  root /home/ubuntu"
                              "/sites/foo/;\n  index index.html index"
                              ".php;\n}\n";
  EXPECT_EQ(target_config, serialized_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, UnmatchingQuotesConfig) {
  bool success = parser.Parse("config_quotes", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, GetConfigOptionsTest) {
  ConfigOptions* options_real = new ConfigOptions();
  parser.Parse("cofig_file_test", &out_config);
  out_config.GetConfigOptions(options_real);
  bool res = options_true->port == 8080;
  EXPECT_TRUE(res);
}
