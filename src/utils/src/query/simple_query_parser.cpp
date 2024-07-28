#include "../../include/query/simple_query_parser.hpp"
#include "../../include/logs/logger.hpp"

void SimpleQueryParser::parse_(std::string &jsonString) {
  if (!queryData__.empty()) return ;

  auto rawDataMap = decodeToDataMap_(jsonString);

  for (const auto &data : rawDataMap) {
    QueryParserValue newValue {data.second};
    queryData__[data.first] = newValue;
  }
}

void SimpleQueryParser::setQueryMetadata() {
  for (const auto &key : queryData__) {
    queryKeys__.push_back(key.first);
    queryValues__.push_back(key.second);
  }
}

SimpleQueryParser::SimpleQueryParser(std::string &jsonString) {
  parse_(jsonString);
  setQueryMetadata();
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
