#pragma once
#include "../../../utils/include/query/query_parser_value.hpp"
#include "../../observer/include/observable.hpp"

struct DataRepository : public Observable<DataRepository> {
  typedef std::unordered_map<std::string, std::string> AllDataMap;

  enum DataType {
    JSON, SIMPLE_STRING
  };

  virtual const std::string getDatabaseName_() const = 0;
  virtual bool isActive_() = 0;

  virtual void addData_(const std::string &key, 
                        const QueryParserValue &value, 
                        const std::string &db,
                        const DataType &dataType = DataType::JSON) = 0;

  virtual QueryParserValue getData_(const std::string &key, const std::string &db) = 0;
  virtual AllDataMap getAllKeysAndValues_(const std::string &db) = 0;
  virtual void deleteData_(const std::string &key, const std::string &db) = 0;
};
