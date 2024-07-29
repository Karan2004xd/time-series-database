#include <filesystem>
#include <gtest/gtest.h>
#include "../../../core/database/include/horizontal_data_indexer.hpp"
#include "../../../core/database/include/rocksdb_data_repository.hpp"

/* TEST(HorizontalDataIndexerSuite, DatabaseInvalidSetupTest) { */
/*   HorizontalDataIndexer indexer; */
/*   RocksDBDataRepository repo; */

/*   EXPECT_ANY_THROW({ */
/*     indexer.dataChanged_(repo, "test_db"); */
/*   }); */
/* } */

/* TEST(HorizontalDataIndexerSuite, DatabaseValidSetupTest) { */
/*   HorizontalDataIndexer indexer; */
/*   RocksDBDataRepository repo; */
/*   std::string filePath = "/tmp/test_db"; */

/*   ASSERT_NO_THROW({ */
/*     repo.addData_("key", {"value"}, "test_db"); */
/*     repo.addData_("key", {"value"}, "test_db/metadata"); */
/*     indexer.dataChanged_(repo, "test_db"); */
/*   }); */

/*   std::filesystem::remove_all(filePath); */
/* } */

/* TEST(HorizontalDataIndexerSuite, DataChangedTest) { */
/* } */
