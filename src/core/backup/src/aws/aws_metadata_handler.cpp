#include "../../include/aws/aws_metadata_handler.hpp"

AWSMetadataHandler::AWSMetadataHandler(DataRepository &repo)
  : repo__(repo) {
  Aws::InitAPI(options__);
}

void AWSMetadataHandler::setBucketsInfo__() {

}

AWSMetadataHandler::~AWSMetadataHandler() {
  Aws::ShutdownAPI(options__);
}
