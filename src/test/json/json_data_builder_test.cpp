#include <gtest/gtest.h>
#include "../../utils/include/json/json_data_builder.hpp"

TEST(JsonDataBuilderSuite, SingleValueTest) {
  JsonDataBuilder builder;

  JsonDataBuilder::JsonGenericParams params {
    {"key_1", "hello"},
    {"key_2", static_cast<long int>(10)},
    {"key_3", static_cast<long double>(34.888)},
  };

  auto data = builder
    .set_("key_1", "hello")
    ->set_("key_2", static_cast<long int>(10))
    ->set_("key_3", static_cast<long double>(34.888))
    ->build_();
  
  ASSERT_EQ(data, params);
}
