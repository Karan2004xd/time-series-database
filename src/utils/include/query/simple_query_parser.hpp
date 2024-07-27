#pragma once

#include "query_parser.hpp"

class SimpleQueryParser : public QueryParser {
public:
  void parse_(const std::string &jsonString) override;

  SimpleQueryParser(const std::string &jsonString);
};
