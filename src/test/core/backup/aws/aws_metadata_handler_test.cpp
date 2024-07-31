#include <aws/core/client/ClientConfiguration.h>
#include <gtest/gtest.h>
#include <aws/s3/S3Client.h>
#include <aws/core/Aws.h>

#include "../../../../core/backup/include/aws/aws_metadata_handler.hpp"

Aws::SDKOptions options;
Aws::Client::ClientConfiguration config;

TEST(AWSMetadataHandlerSuite, GetDefaultRegionTest) {
  Aws::InitAPI(options);

  AWSMetadataHandler metadataHandler;
  ASSERT_EQ(config.region, metadataHandler.getDefaultRegion__());

  Aws::ShutdownAPI(options);
}
