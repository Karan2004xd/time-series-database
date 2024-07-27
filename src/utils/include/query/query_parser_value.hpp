#pragma once

#include "../../../utils/include/json/json_data_builder.hpp"

class QueryParserValue {
private:
  JsonDataBuilder::JsonGenericParamValue value__;

  template <typename ValueType>
  void setValueHelper__(const ValueType &value) {
    if (!std::holds_alternative<std::monostate>(value__)) {
      value__ = value;
    }
  }

public:
  QueryParserValue();
  QueryParserValue(const JsonDataBuilder::JsonGenericParamValue &value);

  void setValue_(const JsonDataBuilder::JsonGenericParamValue &value);
  const JsonDataBuilder::JsonGenericParamValue getValue_() const;

  bool isString_() const;
  bool isDouble_() const;
  bool isInt_() const;
  bool isArray_() const;

  std::string getString_() const;
  long int getInt_() const;
  long double getDouble_() const;

  std::vector<QueryParserValue> getArray_() const;
};
