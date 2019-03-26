//
// Author: Junheng Hao, Yufei Hu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef MEME_MANAGER
#define MEME_MANAGER

#include <tuple>
#include <mutex>
#include <memory>
#include <vector>
#include <utility>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "db_creation_factory.h"

class MemeHandlerManager{
 public:
  static std::mutex meme_mutex_;
  static MemeHandlerManager& Access();

  MemeHandlerManager() {
    try {
      sqlite_connection = DBCreationFactory::GetConnection("sqlite3");
    } catch (const std::exception& ex) {
      BOOST_LOG_TRIVIAL(error) << "MemeHandlerManager::MemeHandlerManager-> " \
                               << "cannot connect to db: " << ex.what();
    }
  }

  ~MemeHandlerManager() {
    sqlite_connection->Close();
    delete sqlite_connection;
  }

  int InsertMeme(const char* top_text, const char* bottom_text, const char* image_name, const char* image_url);
  int GetMemeLatestID();
  void EditMeme(int id, const char* top_text, const char* bottom_text, const char* image_name, const char* image_url);
  void DeleteMeme(int id);
  std::tuple<int, std::string, std::string, std::string, std::string> GetMeme(int id);
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> GetMemeAll();

 private:
  DBConnection* sqlite_connection;
};

#endif
