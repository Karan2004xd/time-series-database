#pragma once

#include "data_repository.hpp"
#include <rocksdb/db.h>

class RocksDBDataRepository : public DataRepository {
private:
  rocksdb::DB *database__ {nullptr};

  std::string databaseName__;
  std::string currentFilePath__;

  void setCurrentFilePath__(const std::string &path);

  void setDatabaseName__(const std::string &name);

  void checkStatus__(const rocksdb::Status &status);
  void setupDatabase__(const std::string &db);

  long int deserializeStringToInt__(const std::string &value);
  long double deserializeStringToDouble__(const std::string &value);

  bool isInteger__(const std::string &value);
  bool isDouble__(const std::string &value);

  std::string addDataHelper__(const QueryParserValue &value,
                              const std::string &key);
  void closeDatabaseConnection__();

public:
  void addData_(const std::string &key, const QueryParserValue &value, const std::string &db) override;
  QueryParserValue getData_(const std::string &key, const std::string &db) override;
  void deleteData_(const std::string &key, const std::string &db) override;

  const std::string getDatabaseName_() const override { return databaseName__; }
  AllDataMap getAllDataMap(const std::string &db) override;

  ~RocksDBDataRepository();
};
