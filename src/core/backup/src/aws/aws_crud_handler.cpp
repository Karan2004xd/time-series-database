#include "../../include/aws/aws_crud_handler.hpp"
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>

AWSCrudHandler::AWSCrudHandler(DataRepository &repo) {
  metadataHandler__ = std::make_unique<AWSMetadataHandler>(repo);
}

void AWSCrudHandler::putData_(const std::string &key, const std::string &data) {
  std::string bucketName = metadataHandler__->getBucketNameUsingValueHash_(key);
  auto config = metadataHandler__->getS3Config_(bucketName);

  Aws::S3::S3Client s3Client {config};

  Aws::S3::Model::PutObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(key);

  auto dataStream = Aws::MakeShared<Aws::StringStream>("Data");
  *dataStream << data;
  request.SetBody(dataStream);

  auto putObjectOutcome = s3Client.PutObject(request);
  if (putObjectOutcome.IsSuccess()) {
    std::cout << "\nObject uploaded successfully" << std::endl;

    std::cout << "Region: " << config.region << std::endl;
    std::cout << "Bucket Name: " << bucketName << std::endl;
    std::cout << "Object Key: " << key << std::endl;
  } else {
    std::cerr << "Failed to upload object to S3: " <<  putObjectOutcome.GetError() << std::endl;
    throw std::runtime_error("AwsCrudOperHandler Error (putData)");
  }
}

std::string AWSCrudHandler::getData_(const std::string &key) {
  std::string bucketName = metadataHandler__->getBucketNameUsingValueHash_(key);
  auto config = metadataHandler__->getS3Config_(bucketName);
  std::ostringstream oss;

  Aws::S3::S3Client s3Client {config};
  Aws::S3::Model::GetObjectRequest request;

  request.SetBucket(bucketName);
  request.SetKey(key);

  Aws::S3::Model::GetObjectOutcome outcome = s3Client.GetObject(request);

  if (outcome.IsSuccess()) {
    oss << outcome.GetResult().GetBody().rdbuf();
  } else {
    const auto &error = outcome.GetError();
    if (error.GetErrorType() == Aws::S3::S3Errors::INTERNAL_FAILURE ||
      error.GetErrorType() == Aws::S3::S3Errors::SERVICE_UNAVAILABLE) {
      throw std::runtime_error("SERVICE FAILURE");
    }
    std::cerr << "Failed to get the object data: " << outcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (getData)");
  }
  return oss.str();
}

void AWSCrudHandler::deleteData_(const std::string &key) {
  std::string bucketName = metadataHandler__->getBucketNameUsingValueHash_(key);
  auto config = metadataHandler__->getS3Config_(bucketName);
  std::ostringstream oss;

  Aws::S3::S3Client s3Client {config};

  Aws::S3::Model::DeleteObjectRequest request;
  request.WithKey(key).WithBucket(bucketName);

  Aws::S3::Model::DeleteObjectOutcome outcome = s3Client.DeleteObject(request);

  if (outcome.IsSuccess()) {
    std::cout << "\nSuccessfully deleted object" << std::endl;

    std::cout << "Region: " << config.region << std::endl;
    std::cout << "Bucket Name: " << bucketName << std::endl;
    std::cout << "Object Key: " << key << std::endl;
  } else {
    std::cerr << "\nFailed to delete the object: " << outcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (deleteData)\n");
  }
}
