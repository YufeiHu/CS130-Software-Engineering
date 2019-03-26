//
// Author: Zhechen Xu, Yufei Hu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef DB_CONNECTION
#define DB_CONNECTION

#include <tuple>
#include <string>
#include <vector>

class DBConnection {
 public:
  virtual ~DBConnection() = default;
  virtual void Close() = 0;
  virtual void DeleteMeme(int id) = 0;
  virtual std::vector<std::tuple<int, std::string, std::string, std::string>> SearchMeme(const char* text_content) = 0;
  virtual void InsertMeme(const char* top_text, const char* bottom_text, const char* image_name, const char* image_url) = 0;
  virtual void EditMeme(int id, const char* top_text, const char* bottom_text, const char* image_name, const char* image_url) = 0;
  virtual int GetMemeLatestID() = 0;
  virtual std::tuple<int, std::string, std::string, std::string, std::string> GetMeme(int id) = 0;
  virtual std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> GetMemeAll() = 0;
};

#endif
