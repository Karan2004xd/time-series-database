#include <gtest/gtest.h>
#include "../../../../core/backup/include/aws/aws_crud_handler.hpp"
#include "../../../../core/database/include/rocksdb_data_repository.hpp"

struct MockClass : public AWSCrudHandler {
  void putData(const std::string &key, const std::string &value) {
    putData_(key, value);
  }

  std::string getData(const std::string &key) {
    return getData_(key);
  }

  void deleteData(const std::string &key) {
    deleteData_(key);
  }
};

TEST(AWSCrudHandlerTestSuite, PutDataTest) {
  RocksDBDataRepository repository;
  MockClass mockClass {repository};

  std::string key = "key";
  std::string data = "dummy test value";
  
  ASSERT_NO_THROW({
    mockClass.putData(key, data);
  });
}

TEST(AWSCrudHandlerTestSuite, GetDataTest) {
  RocksDBDataRepository repository;
  MockClass mockClass {repository};

  std::string key = "key";
  std::string data = "dummy test value";

  ASSERT_NO_THROW({
    std::string result = mockClass.getData(key);

    ASSERT_EQ(result, data);
  });
}

TEST(AWSCrudHandlerTestSuite, InvalidGetDataTest) {
  RocksDBDataRepository repository;
  MockClass mockClass {repository};

  std::string key = "invalid_key";
  std::string data = "dummy test value";

  ASSERT_ANY_THROW({
    std::string result = mockClass.getData(key);
  });
}

TEST(AWSCrudHandlerTestSuite, DeleteDataTest) {
  RocksDBDataRepository repository;
  MockClass mockClass {repository};

  std::string key = "key";

  ASSERT_NO_THROW({
    mockClass.deleteData(key);
  });

  ASSERT_ANY_THROW({
    mockClass.getData(key);
  });
}
