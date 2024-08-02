#pragma once
#include "../../../database/include/data_repository.hpp"

class AWSMetadataHandler {
private:
  DataRepository &repo__;
  std::unordered_map<std::string, std::string> bucketsInfo__;

  void setBucketsInfo__();

public:
  AWSMetadataHandler(DataRepository &repo) : repo__(repo) {}

  const std::string getDefaultRegion_() const;
  const std::vector<std::string> &getBucketNamesByRegion_(const std::string &region) const;

  const std::string getBucketRegionByName_(const std::string &bucket) const;
  const size_t totalBuckets_() const;
};
