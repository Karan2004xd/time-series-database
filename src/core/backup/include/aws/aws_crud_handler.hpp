#pragma once

#include "aws_metadata_handler.hpp"

class AWSCrudHandler {
private:
  std::unique_ptr<AWSMetadataHandler> metadataHandler__;

protected:
  void putData_(const std::string &key, const std::string &data);
  std::string getData_(const std::string &key);
  void deleteData_(const std::string &key);

public:
  AWSCrudHandler() = delete;
  AWSCrudHandler(DataRepository &repo);
};
