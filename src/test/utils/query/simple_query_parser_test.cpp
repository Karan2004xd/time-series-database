#include <gtest/gtest.h>
#include "../../../utils/include/query/simple_query_parser.hpp"

TEST(SimpleQueryParserSuite, DataParsingTest) {
  std::string testString = "{\"key\":\"data\"}";

  SimpleQueryParser parser;
  parser.parse_(testString);
  ASSERT_TRUE(parser.getKeyValue_("key").isString_());
  ASSERT_FALSE(parser.getKeyValue_("key").isInt_());
}

TEST(SimpleQueryParserSuite, DataParsingWithConstructorTest) {
  std::string testString = "{\"key\":\"data\"}";

  SimpleQueryParser parser {testString};

  ASSERT_TRUE(parser.getKeyValue_("key").isString_());
  ASSERT_FALSE(parser.getKeyValue_("key").isInt_());
}

TEST(SimpleQueryParserSuite, InvalidKeyAccessTest) {
  std::string testString = "{\"key\":\"data\"}";

  SimpleQueryParser parser {testString};

  EXPECT_ANY_THROW({
    parser.getKeyValue_("key_1");
  });
}

TEST(SimpleQueryParserSuite, InvalidQueryTest) {
  std::string testString = "{\"key\":\"value\",\"key_1\": {\"key_2\": \"value\"}}";

  EXPECT_ANY_THROW({
    SimpleQueryParser parser {testString};
  });
}

TEST(SimpleQueryParserSuite, DataValidationTest) {
  std::string testString = "{\"key\":\"data\"}";

  SimpleQueryParser parser {testString};
  ASSERT_EQ(parser.getKeyValue_("key").getString_(), "data");
}
