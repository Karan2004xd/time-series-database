#pragma once
#include <string>

template <typename ValueType> struct Observer {
  virtual void dataChanged_(ValueType &source, const std::string &args) = 0;
};
