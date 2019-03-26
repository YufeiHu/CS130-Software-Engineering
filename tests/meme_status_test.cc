#include "gtest/gtest.h"
#include "meme_status.h"

class MemeStatusTest : public ::testing::Test {
 protected:
  ServerStatus& meme_status = ServerStatus::getInstance();
  std::vector<std::pair<std::string, int>> requests_logs;
  std::vector<std::pair<std::string, std::string>> handler_names;
};
    
TEST_F(MemeStatusTest, ResetStatusTest){
  meme_status.ResetStatus();
  EXPECT_EQ(meme_status.GetNumRequest(), 0);
}

TEST_F(MemeStatusTest, InsertMemeTest){
  unique_ptr<Meme> meme1 = make_unique<Meme>("index.jpg", "top text 1", "bottom text 1", 0);
  unique_ptr<Meme> meme2 = make_unique<Meme>("index.jpg", "top text 2", "bottom text 2", 1);
  meme_status.InsertMeme(meme1);
  EXPECT_EQ(meme_status.GetNumRequest(), 1);
  meme_status.InsertMeme(meme2);
  EXPECT_EQ(meme_status.GetNumRequest(), 2); 
}

// test multithread to add new meme
TEST_F(MemeStatusTest, MultiThreadTest) {
	// to be implemented
  EXPECT_TRUE(true);
}
