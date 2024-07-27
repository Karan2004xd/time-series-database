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
