#include <gtest/gtest.h>
#include <unordered_set>
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

bool checkIfTwoArrayEqual(const std::vector<QueryParserValue> &arrayOne,
                          const std::vector<QueryParserValue> &arrayTwo) {
  if (arrayOne.size() != arrayTwo.size()) return false;

  for (const auto &arrayValOne : arrayOne) {
    bool flag {false};
    for (const auto &arrayValTwo : arrayTwo) {
      if (arrayValOne.getValue_() == arrayValTwo.getValue_()) {
        flag = true;
        break;
      }
    }
    if (!flag) return false;
  }
  return true;
}

bool checkIfTwoMapsAreEqual(const std::unordered_map<std::string, QueryParserValue> &mapOne,
                            const std::unordered_map<std::string, QueryParserValue> &mapTwo) {
  if (mapOne.size() != mapTwo.size()) return false;

  for (const auto &key : mapOne) {
    if (key.second.isArray_() && mapTwo.at(key.first).isArray_()) {
      auto arrayOne = key.second.getArray_();
      auto arrayTwo = mapTwo.at(key.first).getArray_();

      if (!checkIfTwoArrayEqual(arrayOne, arrayTwo)) return false;

    } else {
      if (key.second.getValue_() != mapOne.at(key.first).getValue_()) return false;
    }
  }
  return true;
}

TEST(SimpleQueryParserSuite, GetAllKeysAndValuesTest) {
  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";

  JsonDataBuilder::JsonGenericParamsVector vec {
    20, 34.89, "hello", "world"
  };

  std::unordered_map<std::string, QueryParserValue> testMap = {
    {"key_1", {"world"}},
    {"key_2", {10}},
    {"key_3", {15.012}},
    {"key_4", {vec}}
  };

  SimpleQueryParser parser {testString};
  ASSERT_TRUE(checkIfTwoMapsAreEqual(parser.getAllKeysAndValues_(), testMap)); 
}

TEST(SimpleQueryParserSuite, GetAllKeysTest) {
  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";
  SimpleQueryParser parser {testString};

  std::vector<std::string> testKeys {"key_1", "key_2", "key_3", "key_4"};
  std::unordered_set<std::string> testSet (testKeys.begin(), testKeys.end());

  std::vector<std::string> resultKeys = parser.getAllKeys_();

  bool result {true};
  for (const auto &key : resultKeys) {
    if (testSet.find(key) == testSet.end()) {
      result = false;
      break;
    }
  }

  ASSERT_TRUE(result);
}

TEST(SimpleQueryParserSuite, GetAllValuesTest) {
  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";
  SimpleQueryParser parser {testString};

  JsonDataBuilder::JsonGenericParamsVector vec {
    20, 34.89, "hello", "world"
  };

  std::vector<QueryParserValue> testVec {
    {vec}, {"world"}, {10}, {15.012}
  };

  ASSERT_TRUE(checkIfTwoArrayEqual(parser.getAllValues_(), testVec));
}

TEST(SimpleQueryParserSuite, GetSizeTest) {
  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";
  size_t testSize = 4;
  SimpleQueryParser parser {testString};

  ASSERT_EQ(parser.getSize(), testSize);
}
