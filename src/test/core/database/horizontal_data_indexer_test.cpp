#include <filesystem>
#include <gtest/gtest.h>
#include "../../../core/database/include/horizontal_data_indexer.hpp"
#include "../../../core/database/include/rocksdb_data_repository.hpp"

TEST(HorizontalDataIndexerSuite, DatabaseInvalidSetupTest) {
  HorizontalDataIndexer indexer;
  RocksDBDataRepository repo;

  EXPECT_ANY_THROW({
    repo.addData_("key", {"value"}, "test_db");
    indexer.dataChanged_(repo, "key");
  });

  std::filesystem::remove_all("/tmp/test_db");
}

TEST(HorizontalDataIndexerSuite, DatabaseValidSetupTest) {
  HorizontalDataIndexer indexer;
  RocksDBDataRepository repo;

  ASSERT_NO_THROW({
    repo.addData_("key", {"value"}, "test_db");
    indexer.dataChanged_(repo, "key_1");
  });

  std::filesystem::remove_all("/tmp/test_db");
}

TEST(HorizontalDataIndexerSuite, DataChangedTest) {
  HorizontalDataIndexer indexer;
  RocksDBDataRepository repo;

  repo.addData_("key", {"value"}, "test_db");
  indexer.dataChanged_(repo, "key_1");

  ASSERT_TRUE(std::filesystem::exists("/tmp/test_db/key"));
  ASSERT_TRUE(std::filesystem::is_directory("/tmp/test_db/key"));
  ASSERT_NO_THROW({
    auto value = repo.getData_("1", "test_db/key");
  });
}
