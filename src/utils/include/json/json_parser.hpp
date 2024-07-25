#pragma once

#include "json_data_builder.hpp"
#include <memory>

class JsonParser {
public:
  std::unique_ptr<JsonDataBuilder> jsonBuilder_;
  JsonParser() : jsonBuilder_(std::make_unique<JsonDataBuilder>()) {}
};
