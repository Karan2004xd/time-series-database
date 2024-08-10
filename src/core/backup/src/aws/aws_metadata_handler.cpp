#include "../../include/aws/aws_metadata_handler.hpp"
#include "../../../../utils/include/constants.hpp"

#include <aws/s3/S3ServiceClientModel.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/BucketLocationConstraint.h>
#include <aws/s3/model/GetBucketLocationRequest.h>
#include <iomanip>
#include <openssl/sha.h>

DataRepository::AllDataMap AWSMetadataHandler::fetchExistingBucketsFromRepo__() const {
  return repo__.getAllKeysAndValues_(std::string(Constants::BUCKETS_METADATA_DB));
}

std::vector<std::string> AWSMetadataHandler::fetchExistingBucketsFromS3__(const Aws::S3::S3Client &s3Client) const {
  Aws::S3::Model::ListBucketsOutcome listBucketsOutcome = s3Client.ListBuckets();

  std::vector<std::string> result;

  if (listBucketsOutcome.IsSuccess()) {
    const auto &buckets = listBucketsOutcome.GetResult().GetBuckets();
    std::vector<Aws::S3::Model::Bucket> bucketList (buckets.begin(), buckets.end());

    for (const auto &bucket : bucketList) {
      std::string bucketName = bucket.GetName();
      result.push_back(bucketName);
    }
  } else {
    throw;
  }

  return result;
}

std::string AWSMetadataHandler::getBucketRegion__(const std::string &bucketName,
                                                  const Aws::S3::S3Client &s3Client) const {
  std::string region;

  Aws::S3::Model::GetBucketLocationRequest locationRequest;
  locationRequest.SetBucket(bucketName);

  auto getBucketLocationOutcome = s3Client.GetBucketLocation(locationRequest);
  if (getBucketLocationOutcome.IsSuccess()) {
    auto locationConstrain = getBucketLocationOutcome.GetResult().GetLocationConstraint();

    if (locationConstrain == Aws::S3::Model::BucketLocationConstraint::NOT_SET) {
      region = Constants::DEFAULT_S3_BUCKET_REGION;
    } else {
      region = Aws::S3::Model::BucketLocationConstraintMapper::GetNameForBucketLocationConstraint(locationConstrain);
    }
  } else {
    throw;
  }
  return region;
}

void AWSMetadataHandler::setBucketInfoInRepo__() {
  for (const auto &bucket : bucketsInfo__) {
    std::string name = bucket.first;
    std::string bucketRegion = bucket.second;

    std::cout << "\nBucket name : " << name << std::endl;
    std::cout << "Bucket region : " << bucketRegion << std::endl;

    repo__.addData_(name, 
                    {bucketRegion}, 
                    std::string(Constants::BUCKETS_METADATA_DB), 
                    DataRepository::DataType::SIMPLE_STRING);
  }
}

void AWSMetadataHandler::setBucketsInfo__() {
  Aws::S3::S3Client s3Client;

  auto existingData = fetchExistingBucketsFromRepo__();
  auto existingBuckets = fetchExistingBucketsFromS3__(s3Client);

  bool isChanged {false};
  if (existingBuckets.size() != existingData.size()) {
    isChanged = true;
    for (const auto &bucket : existingBuckets) {
      try {
        std::string bucketName = existingData.at(bucket);
      } catch (const std::exception &) {
        existingData[bucket] = getBucketRegion__(bucket, s3Client);
      }
    }
  }

  bucketsInfo__ = existingData;

  if (isChanged) {
    setBucketInfoInRepo__();
  }
}

AWSMetadataHandler::AWSMetadataHandler(DataRepository &repo)
: repo__(repo) {
  Aws::InitAPI(options__);
  setBucketsInfo__();
}

const std::string AWSMetadataHandler::getDefaultRegion_() const {
  return std::string(Constants::DEFAULT_S3_BUCKET_REGION);
}

const std::string AWSMetadataHandler::getBucketNameByRegion_(const std::string &region) const {
  std::string result;
  for (const auto &bucket : bucketsInfo__) {
    if (bucket.second == region) {
      result = bucket.first;
      break;
    }
  }
  return result;
}

const std::vector<std::string> AWSMetadataHandler::getBucketNamesByRegion_(const std::string &region) const {
  std::vector<std::string> result;

  for (const auto &bucket : bucketsInfo__) {
    std::string name = bucket.first;
    std::string bucketRegion = bucket.second;

    if (region == bucketRegion) {
      result.push_back(name);
    }
  }
  return result;
}

const std::string AWSMetadataHandler::getBucketRegionByName_(const std::string &bucket) const {
  return bucketsInfo__.at(bucket);
}

const size_t AWSMetadataHandler::totalBuckets_() const {
  return bucketsInfo__.size();
}

Aws::S3::S3ClientConfiguration AWSMetadataHandler::getS3Config_(const std::string &bucketName) const {
  Aws::S3::S3ClientConfiguration s3ClientConfig;
  s3ClientConfig.region = getBucketRegionByName_(bucketName);
  return s3ClientConfig;
}

const std::string AWSMetadataHandler::md5HashFunction__(const std::string &value) const {
  unsigned char result[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char*)value.c_str(), value.length(), result);

  std::ostringstream sout;
  sout << std::hex << std::setfill('0');
  for(long long c: result) {
    sout << std::setw(2) << (long long)c;
  }
  return sout.str();
}

const std::string AWSMetadataHandler::getBucketNameUsingValueHash_(const std::string &value) const {
  std::string hashHex = md5HashFunction__(value);
  unsigned long long hashInt = std::stoull(hashHex.substr(0, 8), nullptr, 16);

  int bucketIndex = hashInt % totalBuckets_();
  std::string bucketName;

  for (const auto &bucket : bucketsInfo__) {
    bucketName = bucket.first;

    if (bucketIndex == 0) break;
    bucketIndex--;
  }

  return bucketName;
}

AWSMetadataHandler::~AWSMetadataHandler() {
  Aws::ShutdownAPI(options__);
}
