#pragma once

#include "../../observer/include/observer.hpp"
#include "../../../utils/include/query/simple_query_parser.hpp"
#include "../../../utils/include/constants.hpp"
#include "data_repository.hpp"
#include <rocksdb/db.h>

class HorizontalDataIndexer : public Observer<DataRepository> {
private:
  SimpleQueryParser parser__;
  rocksdb::DB *database__ {nullptr};

  std::string databaseName__;
  void setDatabaseName__(const std::string &mainDatabaseName);

  void setDatabase__(const std::string &databaseName);
  void checkStatus__(const rocksdb::Status &status);

  std::vector<QueryParserValue> getKeyValueFromWholeData__(const std::string &key);

public:
  // expecting database name in args (only name no path)
  // expecting the data of metadata to be in format
  // fields : {"names" : ["name of field one", "name of field Two"] ... }
  void dataChanged_(DataRepository &source, const std::string &args) override;
};
