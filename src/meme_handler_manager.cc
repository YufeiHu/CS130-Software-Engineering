//
// Author: Junheng Hao, Yufei Hu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "meme_handler_manager.h"

MemeHandlerManager& MemeHandlerManager::Access() {
  static MemeHandlerManager instance;
  return instance;
}

int MemeHandlerManager::InsertMeme(const char* top_text, const char* bottom_text, const char* image_name, const char* image_url) {
  try {
    sqlite_connection->InsertMeme(top_text, bottom_text, image_name, image_url);
    return sqlite_connection->GetMemeLatestID();
  } catch (const std::exception& ex) {
  	BOOST_LOG_TRIVIAL(error) << "MemeHandlerManager::InsertMeme-> " \
                             << "cannot insert " << ex.what();
    return -1;
  }
}

int MemeHandlerManager::GetMemeLatestID() {
  return sqlite_connection->GetMemeLatestID();
}

void MemeHandlerManager::EditMeme(int id, const char* top_text, const char* bottom_text, const char* image_name, const char* image_url) {
  try {
    sqlite_connection->EditMeme(id, top_text, bottom_text, image_name, image_url);
  } catch (const std::exception& ex) {
    BOOST_LOG_TRIVIAL(error) << "MemeHandlerManager::EditMeme-> " \
                             << "cannot edit " << ex.what();
  }
}

void MemeHandlerManager::DeleteMeme(int id) {
  try {
    sqlite_connection->DeleteMeme(id);
  } catch (const std::exception& ex) {
    BOOST_LOG_TRIVIAL(error) << "MemeHandlerManager::DeleteMeme-> " \
                             << "cannot delete " << ex.what();
  }
}

std::tuple<int, std::string, std::string, std::string, std::string> MemeHandlerManager::GetMeme(int id) {
  std::tuple<int, std::string, std::string, std::string, std::string> ans;
  try {
    return sqlite_connection->GetMeme(id);
  } catch (const std::exception& ex) {
    BOOST_LOG_TRIVIAL(error) << "MemeHandlerManager::DeleteMeme-> " \
                             << "cannot delete " << ex.what();
  }
  return ans;
}

std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> MemeHandlerManager::GetMemeAll() {
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> ans;
  try {
    return sqlite_connection->GetMemeAll();
  } catch (const std::exception& ex) {
    BOOST_LOG_TRIVIAL(error) << "MemeHandlerManager::DeleteMeme-> " \
                             << "cannot delete " << ex.what();
  }
  return ans;
}
