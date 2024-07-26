#include <gtest/gtest.h>
#include "../../../utils/include/json/json_parser.hpp"

TEST(JsonParserSuite, EncodingOfAllTypeTest) {
  JsonParser parser;

  auto data = parser.jsonBuilder_
    ->set_("key_1", "world")
    ->set_("key_2", static_cast<long int>(10))
    ->set_("key_3", static_cast<long double>(15.012))
    ->set_("key_4", {
      static_cast<long int>(20),
      static_cast<long double>(34.89),
      "hello", "world"
    }
           )
    ->build_();

  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";
  ASSERT_EQ(parser.encodeToJsonString_(data), testString);
}

TEST(JsonParserSuite, NullDataEncodingTest) {
  JsonParser parser;

  auto data = parser.jsonBuilder_->build_();
  std::string testString = "{}";

  ASSERT_EQ(parser.encodeToJsonString_(data), testString);
}

TEST(JsonParserSuite, EncodingFunctionOverloadingTest) {
  JsonParser parser;

  auto dataOne = parser.jsonBuilder_->set_("key", "data");
  auto dataTwo = parser.jsonBuilder_->set_("key", "data")->build_();

  std::string testString = "{\"key\":\"data\"}";

  ASSERT_EQ(parser.encodeToJsonString_(dataOne), testString);
  ASSERT_EQ(parser.encodeToJsonString_(dataTwo), testString);
}

TEST(JsonParserSuite, DecodingOfAllTypeTest) {
  JsonParser parser;

  JsonDataBuilder::JsonGenericParamsVector vec {
    static_cast<long int>(20),
    static_cast<long double>(34.89),
    "hello", "world"
  };

  JsonDataBuilder::JsonGenericParams params {
    {"key_1", "world"},
    {"key_2", static_cast<long int>(10)},
    {"key_3", static_cast<long double>(15.012)},
    {"key_4", vec}
  };

  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";
  ASSERT_EQ(params, parser.decodeToDataMap_(testString));
}

TEST(JsonParserSuite, InvalidJsonStringDecodingTest) {
  JsonParser parser;

  JsonDataBuilder::JsonGenericParams params;

  std::string invalidJsonTestString = "{\"key\"\"value\"}"; // missing a ':'
  ASSERT_EQ(parser.decodeToDataMap_(invalidJsonTestString), params);
}
