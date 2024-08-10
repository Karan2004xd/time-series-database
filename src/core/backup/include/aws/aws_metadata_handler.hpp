#pragma once
#include "../../../database/include/data_repository.hpp"
#include <aws/core/Aws.h>
#include <aws/s3/S3ClientConfiguration.h>
#include <aws/s3/S3Client.h>

class AWSMetadataHandler {
private:
  DataRepository &repo__;
  std::unordered_map<std::string, std::string> bucketsInfo__;

  Aws::SDKOptions options__;

  const std::string md5HashFunction__(const std::string &value) const;

  DataRepository::AllDataMap fetchExistingBucketsFromRepo__() const;
  std::vector<std::string> fetchExistingBucketsFromS3__(const Aws::S3::S3Client &s3Client) const;
  std::string getBucketRegion__(const std::string &bucketName, const Aws::S3::S3Client &s3Client) const;

  void setBucketInfoInRepo__();
  void setBucketsInfo__();

public:
  AWSMetadataHandler(DataRepository &repo);

  const std::string getDefaultRegion_() const;

  const std::string getBucketNameByRegion_(const std::string &region) const;
  const std::vector<std::string> getBucketNamesByRegion_(const std::string &region) const;

  const std::string getBucketRegionByName_(const std::string &bucket) const;
  const size_t totalBuckets_() const;

  Aws::S3::S3ClientConfiguration getS3Config_(const std::string &bucketName) const;
  const std::string getBucketNameUsingValueHash_(const std::string &value) const;

  ~AWSMetadataHandler();
};
