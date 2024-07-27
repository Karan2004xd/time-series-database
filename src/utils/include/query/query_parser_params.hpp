#pragma once
#include <string>

class QueryParserParams {
  bool isString_(const std::string &key) const;
  bool isDouble_(const std::string &key) const;
  bool isInt_(const std::string &key) const;
  bool isArray_(const std::string &key) const;

  std::string getString_(const std::string &key) const;
  long int getInt_(const std::string &key) const;
  long double getDouble_(const std::string &key) const;
};
