#pragma once

#include "../json/json_parser.hpp"

struct QueryParser : protected JsonParser {
  virtual void parse_(const std::string &jsonString) = 0;
};
