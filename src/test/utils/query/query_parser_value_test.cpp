#include <gtest/gtest.h>
#include "../../../utils/include/query/query_parser_value.hpp"

TEST(QueryParserValueSuite, ValueInitializationTest) {
  QueryParserValue value {"hello"};
  ASSERT_TRUE(std::holds_alternative<std::string>(value.getValue_()));
}

TEST(QueryParserValueSuite, NoArgsConstructorInitializationTest) {
  QueryParserValue value;
  ASSERT_TRUE(std::holds_alternative<std::monostate>(value.getValue_()));
}

TEST(QueryParserValueSuite, IsStringTest) {
  QueryParserValue value {"hello"};
  ASSERT_TRUE(std::holds_alternative<std::string>(value.getValue_()));
  ASSERT_TRUE(value.isString_());
}

TEST(QueryParserValueSuite, IsIntTest) {
  QueryParserValue value {10020};
  ASSERT_TRUE(std::holds_alternative<long int>(value.getValue_()));
  ASSERT_TRUE(value.isInt_());
}

TEST(QueryParserValueSuite, IsDoubleTest) {
  QueryParserValue value {10.10091};
  ASSERT_TRUE(std::holds_alternative<long double>(value.getValue_()));
  ASSERT_TRUE(value.isDouble_());
}

TEST(QueryParserValueSuite, IsArrayTest) {
  JsonDataBuilder::JsonGenericParamsVector vec {
    static_cast<long int>(10),
    static_cast<long double>(34.888),
    "hello world"
  };

  QueryParserValue value {vec};

  ASSERT_TRUE(std::holds_alternative<JsonDataBuilder::JsonGenericParamsVector>(value.getValue_()));
  ASSERT_TRUE(value.isArray_());
}

TEST(QueryParserValueSuite, GetStringTest) {
  auto testValue = "hello";
  QueryParserValue value {testValue};
  ASSERT_EQ(testValue, value.getString_());
}

TEST(QueryParserValueSuite, GetDoubleTest) {
  auto testValue = 10.0017;
  QueryParserValue value {testValue};
  ASSERT_EQ(testValue, value.getDouble_());
}

TEST(QueryParserValueSuite, GetIntTest) {
  auto testValue = 40019;
  QueryParserValue value {testValue};
  ASSERT_EQ(testValue, value.getInt_());
}

bool checkIfTwoVectorsAreEqual(const std::vector<QueryParserValue> &vecOne,
                               const std::vector<QueryParserValue> &vecTwo) {
  if (vecOne.size() != vecTwo.size()) return false;
  for (size_t i = 0; i < vecOne.size(); i++) {
    if (vecOne[i].getValue_() != vecTwo[i].getValue_()) return false;
  }
  return true;
}

TEST(QueryParserValueSuite, GetArrayTest) {
  JsonDataBuilder::JsonGenericParamsVector testValue {
    static_cast<long int>(10),
    static_cast<long double>(34.888),
    "hello world"
  };

  std::vector<QueryParserValue> resultValue;

  for (const auto &val : testValue) {
    QueryParserValue value;

    if (std::holds_alternative<std::string>(val)) {
      std::string tempVal = std::get<std::string>(val);
      value = QueryParserValue(tempVal);
    } else if (std::holds_alternative<long int>(val)) {
      long int tempVal = std::get<long int>(val);
      value = QueryParserValue(tempVal);
    } else if (std::holds_alternative<long double>(val)) {
      long int tempVal = std::get<long double>(val);
      value = QueryParserValue(tempVal);
    }

    resultValue.push_back(value);
  }

  QueryParserValue value {testValue};
  ASSERT_TRUE(checkIfTwoVectorsAreEqual(resultValue, value.getArray_()));
}

TEST(QueryParserValueSuite, InvalidVariableTypeAccess) {
  QueryParserValue value {"test_string"};
  ASSERT_THROW({
    value.getInt_();
  }, std::bad_variant_access);
}

TEST(QueryParserValueSuite, ReassigningMainValueTest) {
  QueryParserValue value {"test_string"};
  value.setValue_(10);

  ASSERT_FALSE(value.isInt_());
  ASSERT_TRUE(value.isString_());
}
