//
// Author: Zhechen Xu, Yufei Hu
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "sqlite_connection.h"

void SQLiteConnection::Close() {
  sqlite3_close(db);
  BOOST_LOG_TRIVIAL(info) << "SQLiteConnection::Close-> " \
                          << "successfully closed SQLite DB: " << db_name;
}

void SQLiteConnection::InsertMeme(const char* top_text,
                                  const char* bottom_text,
                                  const char* image_name,
                                  const char* image_url) {
  sqlite3_prepare_v2(db, "INSERT INTO meme_info (top_text, bottom_text, image_name, image_url) VALUES (?1, ?2, ?3, ?4);", -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, top_text, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, bottom_text, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, image_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, image_url, -1, SQLITE_STATIC);
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    BOOST_LOG_TRIVIAL(error) << "SQLiteConnection::InsertMeme-> " \
                             << "cannot insert data";
    char *query = NULL;
    asprintf(&query, "ERROR: cannot insert data: %s\n", sqlite3_errmsg(db));
    std::string error = query;
    free(query);
    throw std::invalid_argument(error);
  }
  sqlite3_finalize(stmt);
  BOOST_LOG_TRIVIAL(info) << "SQLiteConnection::InsertMeme-> " \
                          << "successfully inserted: [" << top_text \
                          << ", " << bottom_text \
                          << ", " << image_name \
                          << ", " << image_url << "]";
}

int SQLiteConnection::GetMemeLatestID() {
  return sqlite3_last_insert_rowid(db);
}

void SQLiteConnection::DeleteMeme(int id) {
  sqlite3_prepare_v2(db, "DELETE FROM meme_info WHERE meme_id is ?;", -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, id);
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    BOOST_LOG_TRIVIAL(error) << "SQLiteConnection::DeleteMeme-> " \
                             << "cannot delete data";
    char *query = NULL;
    asprintf(&query, "ERROR: cannot delete data: %s\n", sqlite3_errmsg(db));
    std::string error = query;
    free(query);
    throw std::invalid_argument(error);
  }
  sqlite3_finalize(stmt);
  BOOST_LOG_TRIVIAL(info) << "SQLiteConnection::DeleteMeme-> " \
                          << "successfully deleted";
}

void SQLiteConnection::EditMeme(int id,
                                const char* top_text,
                                const char* bottom_text,
                                const char* image_name,
                                const char* image_url) {
  sqlite3_prepare_v2(db, "UPDATE meme_info SET top_text = ?1, bottom_text = ?2, image_name = ?3, image_url = ?4 WHERE meme_id = ?5;", -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, top_text, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, bottom_text, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, image_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, image_url, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 5, id);
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    BOOST_LOG_TRIVIAL(error) << "SQLiteConnection::EditMeme-> " \
                             << "cannot edit data";
    char *query = NULL;
    asprintf(&query, "ERROR: cannot edit data: %s\n", sqlite3_errmsg(db));
    std::string error = query;
    free(query);
    throw std::invalid_argument(error);
  }
  sqlite3_finalize(stmt);
  BOOST_LOG_TRIVIAL(info) << "SQLiteConnection::EditMeme-> " \
                          << "successfully edited";
}

std::vector<std::tuple<int, std::string, std::string, std::string>> SQLiteConnection::SearchMeme(const char* text_content) {
  sqlite3_prepare_v2(db, "SELECT meme_id, top_text, bottom_text, image_name FROM meme_info;", -1, &stmt, NULL);
  std::vector<std::tuple<int, std::string, std::string, std::string>> res;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int meme_id = sqlite3_column_int(stmt, 0);
    std::string top_text(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string bottom_text(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    std::string image_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    res.push_back(std::make_tuple(meme_id, top_text, bottom_text, image_name));
  }
  if (rc != SQLITE_DONE) {
    BOOST_LOG_TRIVIAL(error) << "SQLiteConnection::SearchMeme-> " \
                             << "cannot fetch data";
    char *query = NULL;
    asprintf(&query, "ERROR: cannot fetch data: %s\n", sqlite3_errmsg(db));
    std::string error = query;
    free(query);
    throw std::invalid_argument(error);
  }
  sqlite3_finalize(stmt);
  BOOST_LOG_TRIVIAL(info) << "SQLiteConnection::SearchMeme-> " \
                          << "successfully fetched";
  return res;
}

std::tuple<int, std::string, std::string, std::string, std::string> SQLiteConnection::GetMeme(int id) {
  sqlite3_prepare_v2(db, "SELECT * FROM meme_info WHERE meme_id is ? LIMIT 1;", -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, id);
  std::tuple<int, std::string, std::string, std::string, std::string> ans;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int meme_id = sqlite3_column_int(stmt, 0);
    std::string top_text(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string bottom_text(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    std::string image_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    std::string image_url(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    ans = std::make_tuple(meme_id, top_text, bottom_text, image_name, image_url);
  }
  if (rc != SQLITE_DONE) {
    char *query = NULL;
    asprintf(&query, "ERROR: cannot get data: %s\n", sqlite3_errmsg(db));
    std::string error = query;
    free(query);
    throw std::invalid_argument(error);
  }
  sqlite3_finalize(stmt);
  return ans;
}

std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> SQLiteConnection::GetMemeAll() {
  sqlite3_prepare_v2(db, "SELECT * FROM meme_info;", -1, &stmt, NULL);
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> ans;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int meme_id = sqlite3_column_int(stmt, 0);
    std::string top_text(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string bottom_text(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    std::string image_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    std::string image_url(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    ans.push_back(std::make_tuple(meme_id, top_text, bottom_text, image_name, image_url));
  }
  if (rc != SQLITE_DONE) {
    char *query = NULL;
    asprintf(&query, "ERROR: cannot get data: %s\n", sqlite3_errmsg(db));
    std::string error = query;
    free(query);
    throw std::invalid_argument(error);
  }
  sqlite3_finalize(stmt);
  return ans;
}
