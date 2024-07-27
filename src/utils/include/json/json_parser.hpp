#pragma once

#include "json_data_builder.hpp"

struct JsonParser {
  virtual std::string encodeToJsonString_(JsonDataBuilder::JsonGenericParams &jsonDataMap) = 0;
  virtual std::string encodeToJsonString_(const JsonDataBuilder *jsonBuilderPtr) = 0;
  virtual JsonDataBuilder::JsonGenericParams decodeToDataMap_(std::string &jsonString) = 0;
};
