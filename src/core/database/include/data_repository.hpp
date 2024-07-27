#pragma once
#include "../../../utils/include/query/query_parser_value.hpp"

struct DataRepository {
  virtual void addData_(const std::string &key, const QueryParserValue &value, const std::string &db) = 0;
  virtual QueryParserValue getData_(const std::string &key, const std::string &db) = 0;
  virtual void deleteData_(const std::string &key, const std::string &db) = 0;
};
