#pragma once

#include "query_parser_value.hpp"

struct QueryParser {
  virtual void parse_(std::string &jsonString) = 0;
  virtual QueryParserValue getKeyValue_(const std::string &key) = 0;

  virtual const std::vector<std::string> &getAllKeys_() const = 0;
  virtual const std::vector<QueryParserValue> &getAllValues_() const = 0;
};
