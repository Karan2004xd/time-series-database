#include <chrono>
#include <filesystem>
#include <future>
#include <gtest/gtest.h>
#include <thread>
#include "../../../core/database/include/rocksdb_data_repository.hpp"

TEST(RocksDBDataRepositorySuite, AddDataTest) {
  RocksDBDataRepository repo;
  QueryParserValue value {"value"};
  std::string filePath = "/tmp/test_db";

  repo.addData_("key", value, "test_db");

  rocksdb::DB *db {nullptr};
  rocksdb::Options options;

  rocksdb::Status status = rocksdb::DB::Open(options, filePath, &db);
  assert(status.ok());

  std::string dbValue;
  status = db->Get(rocksdb::ReadOptions(), "key", &dbValue);
  delete db;

  ASSERT_TRUE(status.ok());
  ASSERT_EQ(dbValue, value.getString_());

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

  ASSERT_EQ(repo.getData_("key", "test_db").getString_(), "value");
  EXPECT_ANY_THROW({
    repo.getData_("key_1", "test_db");
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

TEST(RocksDBDataRepositorySuite, ComplexDataQueryTest) {
  RocksDBDataRepository repo;
  std::string filePath = "/tmp/test_db";

  std::string testString = "{\"key_4\":[20,34.89,\"hello\",\"world\"],\"key_3\":15.012,\"key_2\":10,\"key_1\":\"world\"}";

  repo.addData_("key", {testString}, "test_db");
  ASSERT_EQ(testString, repo.getData_("key", "test_db").getString_());

  std::filesystem::remove_all(filePath);
}
