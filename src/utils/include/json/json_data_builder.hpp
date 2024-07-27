#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class JsonDataBuilder {
public:
  typedef std::vector<std::variant<std::string, long int, long double>> JsonGenericParamsVector;
  typedef std::variant<std::monostate, std::string, long int, long double, JsonGenericParamsVector> JsonGenericParamValue;
  typedef std::unordered_map<std::string, JsonGenericParamValue> JsonGenericParams;

private:
  JsonGenericParams jsonGenericParams__;

  void setJsonParamValue__(const std::string &key, const JsonGenericParamValue &value);

public:
  // to avoid undefined reference errors, template method is initialized within header itself
  template <typename ValueType>
  JsonDataBuilder *set_(const std::string &key, const ValueType &value) {
    setJsonParamValue__(key, value);
    return this;
  }

  JsonDataBuilder *set_(const std::string &key, const JsonGenericParamsVector &values);
  
  JsonGenericParams build_() const;
};
