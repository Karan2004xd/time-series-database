#pragma once
#include "../../../database/include/data_repository.hpp"
#include <aws/core/Aws.h>
#include <aws/s3/S3ClientConfiguration.h>

class AWSMetadataHandler {
private:
  DataRepository &repo__;
  std::unordered_map<std::string, std::string> bucketsInfo__;

  Aws::SDKOptions options__;

  QueryParserValue fetchExistingBucketsFromRepo__() const;
  void setBucketsInfo__();

public:
  AWSMetadataHandler(DataRepository &repo);

  const std::string getDefaultRegion_() const;
  const std::vector<std::string> &getBucketNamesByRegion_(const std::string &region) const;

  const std::string getBucketRegionByName_(const std::string &bucket) const;
  const size_t totalBuckets_() const;

  Aws::S3::S3ClientConfiguration getS3Config(const std::string &bucketName) const;
  ~AWSMetadataHandler();
};
