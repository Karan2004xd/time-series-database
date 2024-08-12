#include "../../include/aws/aws_data_backup.hpp"
#include "../../../../utils/include/json/simple_json_parser.hpp"
#include "../../../../utils/include/constants.hpp"

std::pair<std::string, std::string> AWSDataBackup::getKeyValuePair__(std::string &encodedJsonData) {
  SimpleJsonParser parser;
  auto decodedJsonMap = parser.decodeToDataMap_(encodedJsonData);

  std::string key(Constants::BACKUP_AWS_KEY);
  std::string field(Constants::BACKUP_AWS_FIELD);
  std::string value(Constants::BACKUP_AWS_VALUE);

  std::string decodedField = std::get<std::string>(decodedJsonMap.at(field));
  std::string decodedKey = std::get<std::string>(decodedJsonMap.at(key));
  std::string decodedValue = std::get<std::string>(decodedJsonMap.at(value));

  return {(decodedField + "_" + decodedKey), decodedValue};
}

void AWSDataBackup::dataChanged_(DataRepository &source, const std::string &args) {
}
