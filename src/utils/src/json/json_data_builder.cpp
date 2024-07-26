#include "../../include/json/json_data_builder.hpp"

void JsonDataBuilder::setJsonParamValue__(const std::string &key, const JsonGenericParamValue &value) {
  if (jsonGenericParams__.find(key) != jsonGenericParams__.end()) return ; // key is already present
  jsonGenericParams__[key] = value;
}

JsonDataBuilder *JsonDataBuilder::set_(const std::string &key, const JsonGenericParamsVector &values) {
  setJsonParamValue__(key, values);
  return this;
}

JsonDataBuilder::JsonGenericParams JsonDataBuilder::build_() const {
  return jsonGenericParams__;
}
