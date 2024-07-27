#include "../../include/query/query_parser_value.hpp"
#include "../../include/logs/logger.hpp"

QueryParserValue::QueryParserValue() : value__(std::monostate {}) {}

QueryParserValue::QueryParserValue(const JsonDataBuilder::JsonGenericParamValue &value)
: value__(value) {}

void QueryParserValue::setValue_(const JsonDataBuilder::JsonGenericParamValue &value) {
  if (!std::holds_alternative<std::monostate>(value__)) return ;
  setValueHelper__(value);
}

const JsonDataBuilder::JsonGenericParamValue QueryParserValue::getValue_() const {
  return value__;
}

bool QueryParserValue::isString_() const {
  return std::holds_alternative<std::string>(value__);
}

bool QueryParserValue::isDouble_() const {
  return std::holds_alternative<long double>(value__);
}

bool QueryParserValue::isInt_() const {
  return std::holds_alternative<long int>(value__);
}

bool QueryParserValue::isArray_() const {
  return std::holds_alternative<JsonDataBuilder::JsonGenericParamsVector>(value__);
}

std::string QueryParserValue::getString_() const {
  if (isString_()) {
    return std::get<std::string>(value__);
  }
  Logger::log_(Logger::ERROR) << "(getString_) Invalid type of value access\n";
  throw std::bad_variant_access();
}

long int QueryParserValue::getInt_() const {
  if (isInt_()) {
    return std::get<long int>(value__);
  }
  Logger::log_(Logger::ERROR) << "(getInt_) Invalid type of value access\n";
  throw std::bad_variant_access();
}

long double QueryParserValue::getDouble_() const {
  if (isDouble_()) {
    return std::get<long double>(value__);
  }
  Logger::log_(Logger::ERROR) << "(getDouble_) Invalid type of value access\n";
  throw std::bad_variant_access();
}

std::vector<QueryParserValue> QueryParserValue::getArray_() const {
  if (isArray_()) {
    auto tempArray = std::get<JsonDataBuilder::JsonGenericParamsVector>(value__);
    std::vector<QueryParserValue> resultVector;

    for (const auto &val : tempArray) {
      QueryParserValue value;

      if (std::holds_alternative<std::string>(val)) {
        std::string tempVal = std::get<std::string>(val);
        value = QueryParserValue(tempVal);

      } else if (std::holds_alternative<long int>(val)) {
        long int tempVal = std::get<long int>(val);
        value = QueryParserValue(tempVal);

      } else if (std::holds_alternative<long double>(val)) {
        long int tempVal = std::get<long double>(val);
        value = QueryParserValue(tempVal);
      }

      resultVector.push_back(value);
    }

    return resultVector;
  }

  Logger::log_(Logger::ERROR) << "(getDouble_) Invalid type of value access\n";
  throw std::bad_variant_access();
}
