#include <gtest/gtest.h>

#include "../../../../core/backup/include/aws/aws_metadata_handler.hpp"
#include "../../../../core/database/include/rocksdb_data_repository.hpp"
#include "../../../../utils/include/constants.hpp"

RocksDBDataRepository repo;

TEST(AWSMetadataHandlerSuite, GetDefaultRegionTest) {
  AWSMetadataHandler metadataHandler {repo};
  ASSERT_EQ(std::string(Constants::DEFAULT_S3_BUCKET_REGION), metadataHandler.getDefaultRegion_());
}

TEST(AWSMetadataHandlerSuite, GetBucketNamesByRegionTest) {
  AWSMetadataHandler metadataHandler {repo};
  std::string region = "ap-northeast-1";
  auto vec = metadataHandler.getBucketNamesByRegion_(region);

  ASSERT_TRUE(std::find(vec.begin(), vec.end(), region) != vec.end());
  ASSERT_FALSE(std::find(vec.begin(), vec.end(), "dummy_data") != vec.end());
}

TEST(AWSMetadataHandlerSuite, GetBucketRegionByNameTest) {
  AWSMetadataHandler metadataHandler {repo};
  std::string bucketName = std::string(Constants::S3_TEST_BUCKET);
  std::string testRegion = "eu-north-1";

  ASSERT_EQ(testRegion, metadataHandler.getBucketRegionByName_(bucketName));
}

TEST(AWSMetadataHandlerSuite, GetS3ConfigTest) {
  AWSMetadataHandler metadataHandler {repo};
  std::string bucketName = std::string(Constants::S3_TEST_BUCKET);
  std::string testRegion = metadataHandler.getBucketRegionByName_(bucketName);

  ASSERT_NO_THROW({
    ASSERT_EQ(testRegion, metadataHandler.getS3Config(bucketName).region);
  });
}

TEST(AWSMetadataHandlerSuite, GetS3ConfigForInvalidBucketNameTest) {
  AWSMetadataHandler metadataHandler {repo};
  std::string bucketName = "dummy_data";

  ASSERT_ANY_THROW({
    metadataHandler.getS3Config(bucketName);
  });
}
