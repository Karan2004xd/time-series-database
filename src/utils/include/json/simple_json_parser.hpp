#pragma once

#include "json_parser.hpp"
#include <memory>
#include <rapidjson/document.h>

class SimpleJsonParser : public JsonParser {
private:
  JsonDataBuilder::JsonGenericParamsVector getListValues__(const rapidjson::Value::ConstArray &dataList);
public:
  std::unique_ptr<JsonDataBuilder> jsonBuilder_;
  SimpleJsonParser() : jsonBuilder_(std::make_unique<JsonDataBuilder>()) {}

  std::string encodeToJsonString_(JsonDataBuilder::JsonGenericParams &jsonDataMap) override;
  std::string encodeToJsonString_(const JsonDataBuilder *jsonBuilderPtr) override;

  JsonDataBuilder::JsonGenericParams decodeToDataMap_(std::string &jsonString) override;
};
