//
// Author: Zhechen Xu, Yufei Hu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef SQLITE_CONNECTION
#define SQLITE_CONNECTION

#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "sqlite_util.h"
#include "db_connection.h"

class SQLiteConnection : public DBConnection {
 private:
  sqlite3* db;
  const char* db_name;
  int rc;
  sqlite3_stmt *stmt;

 public:
  SQLiteConnection() {
    db_name = SQLiteUtil::DB_NAME.c_str();
    rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "SQLiteConnection::SQLiteConnection-> " \
                               << "cannot open SQLite DB: " << db_name;
      char *query = NULL;
      asprintf(&query, "ERROR: cannot open SQLite DB %s: %s\n", db_name, sqlite3_errmsg(db));
      std::string error = query;
      free(query);
      throw std::invalid_argument(error);
    }
    BOOST_LOG_TRIVIAL(info) << "SQLiteConnection::SQLiteConnection-> " \
                            << "successfully opened SQLite DB: " << db_name;
  }
  void Close();
  void DeleteMeme(int id);
  std::vector<std::tuple<int, std::string, std::string, std::string>> SearchMeme(const char* text_content);
  void InsertMeme(const char* top_text, const char* bottom_text, const char* image_name, const char* image_url);
  void EditMeme(int id, const char* top_text, const char* bottom_text, const char* image_name, const char* image_url);
  int GetMemeLatestID();
  std::tuple<int, std::string, std::string, std::string, std::string> GetMeme(int id);
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> GetMemeAll();
};

#endif
