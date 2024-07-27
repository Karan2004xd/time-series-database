#pragma once

#include "query_parser.hpp"

class SimpleQueryParser : public QueryParser {
private:
public:
  SimpleQueryParser(const std::string &jsonString);

  void parse_(const std::string &jsonString) override;
  QueryParserValue getKeyValue_(const std::string &key) override;
};
