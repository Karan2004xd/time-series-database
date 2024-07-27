#pragma once

#include "query_parser_value.hpp"

struct QueryParser {
  virtual void parse_(std::string &jsonString) = 0;
  virtual QueryParserValue getKeyValue_(const std::string &key) = 0;
};
