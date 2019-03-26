//
// Author: Zhechen Xu
// ~~~~~~~~~~~~~~~~~~
//

#ifndef DB_CREATION_FACTORY
#define DB_CREATION_FACTORY

#include <string>
#include <iostream>

#include "db_connection.h"

class DBCreationFactory {
 private:
  static const std::string DEFAULT_DB;
  
 public:
  static DBConnection* GetConnection(std::string db);
  static DBConnection* GetConnection();
};

#endif
