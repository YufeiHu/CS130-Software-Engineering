//
// Author: Zhechen Xu
// ~~~~~~~~~~~~~~~~~~
//

#include <stdexcept>

#include "sqlite_connection.h"
#include "db_creation_factory.h"

const std::string DBCreationFactory::DEFAULT_DB = "sqlite3";

DBConnection* DBCreationFactory::GetConnection(std::string db) {
  DBConnection* db_connection(NULL);
  if (db.compare("sqlite3") == 0) {
    db_connection = new SQLiteConnection;
  } else if (db.compare("mongodb") == 0) {
    throw std::invalid_argument("Support for Mongodb is currently under development"); 
  } else {
    throw std::invalid_argument("This database type is not supported"); 
  }
  return db_connection;
}
  
DBConnection* DBCreationFactory::GetConnection() {
  return DBCreationFactory::GetConnection(DBCreationFactory::DEFAULT_DB);
}
