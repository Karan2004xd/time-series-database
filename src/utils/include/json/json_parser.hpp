#pragma once

#include "json_data_builder.hpp"
#include <memory>
#include <rapidjson/document.h>

class JsonParser {
private:
  JsonDataBuilder::JsonGenericParamsVector getListValues__(const rapidjson::Value::ConstArray &dataList);
public:
  std::unique_ptr<JsonDataBuilder> jsonBuilder_;
  JsonParser() : jsonBuilder_(std::make_unique<JsonDataBuilder>()) {}

  std::string encodeToJsonString_(JsonDataBuilder::JsonGenericParams &jsonDataMap);
  std::string encodeToJsonString_(const JsonDataBuilder *jsonBuilderPtr);

  JsonDataBuilder::JsonGenericParams decodeToDataMap_(std::string &jsonString);
};
