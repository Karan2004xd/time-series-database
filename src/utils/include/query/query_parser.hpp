#pragma once

#include "../json/json_parser.hpp"
#include "query_parser_value.hpp"

struct QueryParser : protected JsonParser {
  virtual void parse_(const std::string &jsonString) = 0;
  virtual QueryParserValue getKeyValue_(const std::string &key) = 0;
};
