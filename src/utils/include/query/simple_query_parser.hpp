#pragma once

#include "query_parser.hpp"
#include "../../include/json/simple_json_parser.hpp"

// Does not support nested queries 
class SimpleQueryParser : public QueryParser, protected SimpleJsonParser {
private:
  std::string currentJsonString__;

  std::unordered_map<std::string, QueryParserValue> queryData__;
  std::vector<std::string> queryKeys__;
  std::vector<QueryParserValue> queryValues__;

  void setQueryMetadata();
  
public:
  SimpleQueryParser() {}
  SimpleQueryParser(std::string &jsonString);

  void parse_(std::string &jsonString) override;
  QueryParserValue getKeyValue_(const std::string &key) override;

  const std::vector<std::string> &getAllKeys_() const override;
  const std::vector<QueryParserValue> &getAllValues_() const override;

  const std::unordered_map<std::string, QueryParserValue> &getAllKeysAndValues_() const { return queryData__;}
  const size_t getSize() const { return queryData__.size(); }

  const bool containesKey(const std::string &key) const;
};
