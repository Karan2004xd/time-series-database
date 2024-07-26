#include <gtest/gtest.h>
#include <limits>
#include "../../utils/include/json/json_data_builder.hpp"

TEST(JsonDataBuilderSuite, SingleValueTest) {
  JsonDataBuilder builder;

  JsonDataBuilder::JsonGenericParamsVector vec {
    static_cast<long int>(10),
    static_cast<long double>(34.888),
    "hello world"
  };

  JsonDataBuilder::JsonGenericParams params {
    {"key_1", "hello"},
    {"key_2", static_cast<long int>(10)},
    {"key_3", static_cast<long double>(34.888)},
    {"key_4", vec}
  };

  auto data = builder
    .set_("key_1", "hello")
    ->set_("key_2", static_cast<long int>(10))
    ->set_("key_3", static_cast<long double>(34.888))
    ->set_("key_4", {
      static_cast<long int>(10),
      static_cast<long double>(34.888),
      "hello world"
    }
           )
    ->build_();
  
  ASSERT_EQ(data, params);
}

TEST(JsonDataBuilderSuite, SingleKeyTest) {
  JsonDataBuilder builder;

  JsonDataBuilder::JsonGenericParams params {
    {"key_1", "hello"},
  };

  auto data = builder
    .set_("key_1", "hello")
    ->set_("key_1", static_cast<long int>(10))
    ->set_("key_1", static_cast<long double>(34.888))
    ->set_("key_1", {
      static_cast<long int>(10),
      static_cast<long double>(34.888),
      "hello world"
    }
           )
    ->build_();
  
  ASSERT_EQ(data, params);
}

TEST(JsonDataBuilderSuite, EmptyVectorTest) {
  JsonDataBuilder builder;

  JsonDataBuilder::JsonGenericParamsVector vec;

  JsonDataBuilder::JsonGenericParams params {
    {"key_1", vec}
  };

  auto data = builder
    .set_("key_1", {})
    ->build_();
  
  ASSERT_EQ(params["key_1"], data["key_1"]);
  ASSERT_EQ(data, params);
}

TEST(JsonDataBuilderSuite, LargeNumberTest) {
  JsonDataBuilder builder;

  long int longInt = std::numeric_limits<long int>::max();
  long double longDouble = std::numeric_limits<long double>::max();

  auto data = builder
    .set_("large_int", longInt)
    ->set_("large_double", longDouble)
    ->build_();
  
  ASSERT_EQ(std::get<long int>(data["large_int"]), longInt);
  ASSERT_EQ(std::get<long double>(data["large_double"]), longDouble);
}

TEST(JsonDataBuilderSuite, NestedValuesTest) {
  JsonDataBuilder builder;

  JsonDataBuilder::JsonGenericParamsVector vec {
    static_cast<long int>(10),
    static_cast<long double>(34.888),
    "hello world"
  };

  JsonDataBuilder::JsonGenericParams params {
    {"key_1", "hello"},
    {"key_2", static_cast<long int>(10)},
    {"key_3", static_cast<long double>(34.888)},
    {"key_4", vec},
  };

  auto data = builder
    .set_("key_1", "hello")
    ->set_("key_2", static_cast<long int>(10))
    ->set_("key_3", static_cast<long double>(34.888))
    ->set_("key_4", {
      static_cast<long int>(10),
      static_cast<long double>(34.888),
      "hello world"
    }
           )
    ->build_();
  
  ASSERT_EQ(data, params);
}
