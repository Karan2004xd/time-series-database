#include <gtest/gtest.h>
#include "../../../../core/backup/include/aws/aws_data_backup.hpp"

struct MockClass : public AWSDataBackup {
  std::pair<std::string, std::string> getKeyValuePair(std::string &value) {
    return getKeyValuePair__(value);
  }
};

TEST(AWSDataBackupSuite, GetKeyValuePairTest) {
  MockClass mc;
  std::string testString = "{\"field\":\"test_db\",\"key\":\"1\",\"value\":\"value\"}";
  auto result = mc.getKeyValuePair(testString);

  ASSERT_EQ(result.first, "test_db_1");
  ASSERT_EQ(result.second, "value");
}
