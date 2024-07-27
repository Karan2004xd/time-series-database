#pragma once

#include "query_parser.hpp"
#include "../../include/json/simple_json_parser.hpp"

// Does not support nested queries 
class SimpleQueryParser : public QueryParser, protected SimpleJsonParser {
private:
  std::unordered_map<std::string, QueryParserValue> queryData__;
  
public:
  SimpleQueryParser() {}
  SimpleQueryParser(std::string &jsonString);

  void parse_(std::string &jsonString) override;
  QueryParserValue getKeyValue_(const std::string &key) override;
};
