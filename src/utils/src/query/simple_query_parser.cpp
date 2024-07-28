#include "../../include/query/simple_query_parser.hpp"
#include "../../include/logs/logger.hpp"
#include <algorithm>

void SimpleQueryParser::setQueryMetadata() {
  for (const auto &key : queryData__) {
    queryKeys__.push_back(key.first);
    queryValues__.push_back(key.second);
  }
}

void SimpleQueryParser::parse_(std::string &jsonString) {
  if (!queryData__.empty()) return ;

  auto rawDataMap = decodeToDataMap_(jsonString);

  for (const auto &data : rawDataMap) {
    QueryParserValue newValue {data.second};
    queryData__[data.first] = newValue;
  }

  setQueryMetadata();
}

SimpleQueryParser::SimpleQueryParser(std::string &jsonString) {
  parse_(jsonString);
}

QueryParserValue SimpleQueryParser::getKeyValue_(const std::string &key) {
  if (queryData__.find(key) != queryData__.end()) {
    return queryData__[key];
  }
  Logger::log_(Logger::ERROR) << "(getKeyValue_) key (" << key << ") was not found\n";
  throw new std::runtime_error("");
}

const std::vector<std::string> &SimpleQueryParser::getAllKeys_() const {
  return queryKeys__;
}

const std::vector<QueryParserValue> &SimpleQueryParser::getAllValues_() const {
  return queryValues__;
}

const bool SimpleQueryParser::containesKey(const std::string &key) const {
  return std::find(queryKeys__.begin(), queryKeys__.end(), key) != queryKeys__.end();
}
