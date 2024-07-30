#include <chrono>
#include <filesystem>
#include <future>
#include <gtest/gtest.h>
#include <thread>
#include "../../../core/database/include/rocksdb_data_repository.hpp"
#include "../../../core/database/include/horizontal_data_indexer.hpp"

TEST(RocksDBDataRepositorySuite, AddDataTest) {
  RocksDBDataRepository repo;
  QueryParserValue value {"value"};
  std::string filePath = "/tmp/test_db";

  repo.addData_("key", value, "test_db");
  repo.closeDatabaseConnection_();

  rocksdb::DB *db {nullptr};
  rocksdb::Options options;

  rocksdb::Status status = rocksdb::DB::Open(options, filePath, &db);
  assert(status.ok());

  std::string dbValue;
  status = db->Get(rocksdb::ReadOptions(), "key_1", &dbValue);

  ASSERT_TRUE(status.ok());
  ASSERT_EQ(dbValue, value.getString_());
  delete db;

  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, GetDataTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";

  rocksdb::DB *db;
  rocksdb::Options options;
  options.create_if_missing = true;

  rocksdb::Status status = rocksdb::DB::Open(options, filePath, &db);
  assert(status.ok());

  status = db->Put(rocksdb::WriteOptions(), "key", "value");
  delete db;

  QueryParserValue storedValue = repo.getData_("key", "test_db");

  ASSERT_TRUE(status.ok());
  ASSERT_EQ("value", storedValue.getString_());
  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, DeleteDataTest) {
  RocksDBDataRepository repo;
  
  std::string filePath = "/tmp/test_db";

  rocksdb::DB *db, *db2;
  rocksdb::Options options;
  options.create_if_missing = true;

  rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/test_db", &db);
  assert(status.ok());

  status = db->Put(rocksdb::WriteOptions(), "key", "value");
  delete db;

  repo.deleteData_("key", "test_db");
  repo.closeDatabaseConnection_();

  status = rocksdb::DB::Open(options, filePath, &db2);
  assert(status.ok());

  std::string dbValue;
  status = db2->Get(rocksdb::ReadOptions(), "key", &dbValue);

  ASSERT_FALSE(status.ok());
  delete db2;

  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, InvalidDataAccessTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";

  repo.addData_("key", {"value"}, "test_db");

  ASSERT_EQ(repo.getData_("key_1", "test_db").getString_(), "value");
  EXPECT_ANY_THROW({
    repo.getData_("key_2", "test_db");
  });

  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, ConcurrentOperationTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";

  std::vector<std::future<void>> activeThreads;

  auto operation = [&repo](int i) {
    std::string key = "key_" + std::to_string(i);
    std::string value = "value_" + std::to_string(i);

    repo.addData_(key, {value}, "test_db");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(repo.getData_(key, "test_db").getString_(), value);
  };

  for (int i = 0; i < 10; i++) {
    activeThreads.push_back(std::async(std::launch::async, operation, i));
  }

  for (const auto &thread : activeThreads) {
    thread.wait();
  }
  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, IsActiveTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";
  repo.addData_("key", {"value"}, "test_db");

  ASSERT_TRUE(repo.isActive_());

  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, CloseDatabaseConnectionTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";
  repo.addData_("key", {"value"}, "test_db");
  repo.closeDatabaseConnection_();

  ASSERT_TRUE(repo.isActive_());

  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, GetDatabaseNameTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";
  repo.addData_("key", {"value"}, "test_db");

  ASSERT_EQ(filePath, repo.getDatabaseName_());
  std::filesystem::remove_all(filePath);
}

TEST(RocksDBDataRepositorySuite, NotifyObserverTest) {
  RocksDBDataRepository repo;
  HorizontalDataIndexer indexer;

  repo.subscribe_(&indexer);
  QueryParserValue value;

  ASSERT_NO_THROW({
    repo.addData_("key", {"value"}, "test_db");
    value = repo.getData_("1", "test_db/key");
  });

  ASSERT_EQ(value.getString_(), "value");
  std::filesystem::remove_all("/tmp/test_db");
}

TEST(RocksDBDataRepositorySuite, AddingMultipleSimpleDataTest) {
  RocksDBDataRepository repo;
  HorizontalDataIndexer indexer;

  repo.subscribe_(&indexer);

  ASSERT_NO_THROW({
    for (int i = 1; i < 10; i++) {
      std::string stringValue = "value_" + std::to_string(i);
      repo.addData_("values_key", {stringValue}, "test_db");
    }
  });

  std::filesystem::remove_all("/tmp/test_db");
}

TEST(RocksDBDataRepositorySuite, GettingMultipleSimpleDataTest) {
  RocksDBDataRepository repo;
  HorizontalDataIndexer indexer;

  repo.subscribe_(&indexer);
  std::vector<QueryParserValue> values;

  for (int i = 1; i < 10; i++) {
    std::string stringValue = "value_" + std::to_string(i);
    repo.addData_("values_key", {stringValue}, "test_db");
    values.push_back({stringValue});
  }

  for (int i = 1; i < 10; i++) {
    auto value = repo.getData_(std::to_string(i), "test_db/values_key");
    ASSERT_EQ(value.getString_(), values[i - 1].getString_());
  }

  std::filesystem::remove_all("/tmp/test_db");
}

TEST(RocksDBDataRepositorySuite, MultipleConcurrentDataTest) {
  RocksDBDataRepository repo;
  HorizontalDataIndexer indexer;

  repo.subscribe_(&indexer);
  std::vector<QueryParserValue> values;

  auto addDataOperation = [&repo, &values]() {
    for (int i = 1; i < 10; i++) {
      std::string stringValue = "value_" + std::to_string(i);
      repo.addData_("values_key", {stringValue}, "test_db");
      values.push_back({stringValue});
    }
  };

  auto getDataOperation = [&repo, &values]() {
    for (int i = 1; i < 10; i++) {
      auto value = repo.getData_(std::to_string(i), "test_db/values_key");
      std::string resultValue = value.getString_();
      ASSERT_EQ(resultValue, values[i - 1].getString_());
    }
  };

  std::future<void> threadOne = std::async(std::launch::async, addDataOperation);
  std::future<void> threadTwo = std::async(std::launch::async, getDataOperation);

  threadOne.wait();
  threadTwo.wait();

  std::filesystem::remove_all("/tmp/test_db");
}

TEST(RocksDBDataRepositorySuite, MultipleDataTypesTest) {
  RocksDBDataRepository repo;
  HorizontalDataIndexer indexer;

  repo.subscribe_(&indexer);

  repo.addData_("key_1", {"value_1"}, "test_db");
  repo.addData_("key_1", {1023}, "test_db");
  repo.addData_("key_1", {10.101}, "test_db");

  auto valueOne = repo.getData_("1", "test_db/key_1");
  auto valueTwo = repo.getData_("2", "test_db/key_1");
  auto valueThree = repo.getData_("3", "test_db/key_1");

  ASSERT_EQ(valueOne.getString_(), "value_1");
  ASSERT_EQ(valueTwo.getString_(), "1023");
  ASSERT_EQ(valueThree.getString_(), "10.101");

  std::filesystem::remove_all("/tmp/test_db");
}

TEST(RocksDBDataRepositorySuite, ListOfSimilarDataTypesTest) {
  RocksDBDataRepository repo;
  HorizontalDataIndexer indexer;

  repo.subscribe_(&indexer);

  JsonDataBuilder::JsonGenericParamsVector vec {
    "hello", "world", 10.102, 1002
  };

  std::string testString = "{\"vec_key_1\":[\"hello\",\"world\",10,1002]}";
  repo.addData_("vec_key", {vec}, "test_db");

  ASSERT_NO_THROW({
    auto value = repo.getData_("1", "test_db/vec_key");
    ASSERT_EQ(value.getString_(), testString);
  });

  std::filesystem::remove_all("/tmp/test_db");
}
