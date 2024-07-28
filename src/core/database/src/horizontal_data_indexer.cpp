#include "../include/horizontal_data_indexer.hpp"
#include "../../../utils/include/logs/logger.hpp"
#include <filesystem>

void HorizontalDataIndexer::setDatabaseName__(const std::string &mainDatabaseName) {
  databaseName__ = std::string(Constants::DEFAULT_DATABASE_DIRECTORY)
                  + mainDatabaseName + "/"
                  + std::string(Constants::METADATA_DATABASE_NAME);
}

void HorizontalDataIndexer::checkStatus__(const rocksdb::Status &status) {
  if (status.ok()) return ;
  Logger::log_(Logger::ERROR) << "(checkStatus__) rocksdb status was not ok, Error (" << status.ToString() << ")\n";
  throw std::runtime_error(status.ToString());
}

void HorizontalDataIndexer::setDatabase__(const std::string &databaseName) {
  setDatabaseName__(databaseName);

  rocksdb::Options options;
  options.OptimizeLevelStyleCompaction();
  options.IncreaseParallelism();

  rocksdb::Status status = rocksdb::DB::Open(options, databaseName__, &database__);
  checkStatus__(status);
}

std::vector<QueryParserValue> HorizontalDataIndexer::getKeyValueFromWholeData__(const std::string &key) {
  std::vector<QueryParserValue> values;
  SimpleQueryParser parser;

  rocksdb::Iterator *it = database__->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::string value = it->value().ToString();
    parser.parse_(value);
    if (parser.containesKey(key)) {
      values.push_back({value});
    }
  }
  return values;
}

void HorizontalDataIndexer::dataChanged_(DataRepository &source, const std::string &args) {
  setDatabase__(args);
  auto fields = source.getData_(std::string(Constants::METADATA_FIELDS_DATASET_NAME), databaseName__);
  
  if (fields.isString_()) {
    std::string fieldsData = fields.getString_();
    parser__.parse_(fieldsData);

    auto listOfNames = parser__.getKeyValue_(std::string(Constants::FIELDS_JSON_FORMAT_KEY_NAME));
    if (listOfNames.isArray_()) {
      for (const auto &name : listOfNames.getArray_()) {
        std::string nameString = name.getString_();
        std::string filePathToCheck = databaseName__ + "/" + nameString;

        if (!std::filesystem::exists(filePathToCheck) || !std::filesystem::is_directory(filePathToCheck)) {
          auto dataToAdd = getKeyValueFromWholeData__(nameString);

          for (const auto &data : dataToAdd) {
            source.addData_(nameString, data, filePathToCheck);
          }
        }
      }
    }
  }
}
