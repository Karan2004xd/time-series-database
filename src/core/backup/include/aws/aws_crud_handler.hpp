#pragma once

#include "aws_metadata_handler.hpp"
#include <memory>
#include <string>

class AWSCrudHandler {
private:
  std::unique_ptr<AWSMetadataHandler> metadataHandler__;
protected:
  AWSCrudHandler(DataRepository &repo);

  void putData_(const std::string &key, const std::string &data);
  std::string getData_(const std::string &key);
  void deleteData_(const std::string &key);
};
