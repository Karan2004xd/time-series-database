#include "../include/rocksdb_data_repository.hpp"
#include "../../../utils/include/constants.hpp"
#include "../../../utils/include/logs/logger.hpp"

void RocksDBDataRepository::checkStatus__(const rocksdb::Status &status) {
  if (status.ok()) {
    return ;
  }
  Logger::log_(Logger::ERROR) << "(checkStatus__) Status was not ok\n";
  throw std::runtime_error(status.ToString());
}

long int RocksDBDataRepository::deserializeStringToInt__(const std::string &value) {
  try {
    size_t pos;
    long int intValue = std::stol(value, &pos);
    if (pos != value.size()) {
      throw std::invalid_argument({});
    }
    return intValue;
  } catch (const std::exception &) {
    throw;
  }
}

long double RocksDBDataRepository::deserializeStringToDouble__(const std::string &value) {
  try {
    size_t pos;
    long int doubleValue = std::stold(value, &pos);
    if (pos != value.size()) {
      throw std::invalid_argument({});
    }
    return doubleValue;

  } catch (const std::exception &) {
    throw;
  }
}

bool RocksDBDataRepository::isInteger__(const std::string &value) {
  try {
    deserializeStringToInt__(value);
    return true;

  } catch (...) {
    return false;
  }
}

bool RocksDBDataRepository::isDouble__(const std::string &value) {
  try {
    deserializeStringToDouble__(value);
    return true;

  } catch (...) {
    return false;
  }
}

void RocksDBDataRepository::setupDatabase__(const std::string &db) {
  rocksdb::Options options;
  options.create_if_missing = true;
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  
  std::string filePath = std::string(Constants::DEFAULT_DIRECTORY) + db;
  rocksdb::Status status = rocksdb::DB::Open(options, filePath, &database__);

  checkStatus__(status);
}

std::string RocksDBDataRepository::addDataHelper__(const QueryParserValue &value,
                                                   const std::string &key) {
  std::string valueString;

  if (value.isString_()) {
    valueString = value.getString_();

  } else if (value.isInt_()) {
    long int valueToStore = value.getInt_();
    valueString = std::string(reinterpret_cast<const char *>(&valueToStore), sizeof(valueToStore));

  } else if (value.isDouble_()) {
    long double valueToStore = value.getDouble_();
    valueString = std::string(reinterpret_cast<const char *>(&valueToStore), sizeof(valueToStore));

  } else if (value.isArray_()) {
    JsonDataBuilder::JsonGenericParamsVector valuesToStore;

    for (const auto &val : value.getArray_()) {
      if (val.isString_()) {
        valuesToStore.push_back(value.getString_());
      } else if (val.isInt_()) {
        valuesToStore.push_back(value.getInt_());
      } else if (val.isDouble_()) {
        valuesToStore.push_back(value.getDouble_());
      }
    }

    valueString = jsonParser__.encodeToJsonString_(jsonParser__.jsonBuilder_->set_(key, valuesToStore));
  }
  return valueString;
}

void RocksDBDataRepository::addData_(const std::string &key,
                                     const QueryParserValue &value,
                                     const std::string &db) {
  setupDatabase__(db);
  rocksdb::Status status = database__->Put(rocksdb::WriteOptions(), key, addDataHelper__(value, key));

  database__->Close();
  checkStatus__(status);
}

QueryParserValue RocksDBDataRepository::getData_(const std::string &key, const std::string &db) {
  setupDatabase__(db);

  std::string retrivedValue;
  rocksdb::Status status = database__->Get(rocksdb::ReadOptions(), key, &retrivedValue);

  QueryParserValue resultValue;
  if (isDouble__(retrivedValue)) {
    resultValue.setValue_(deserializeStringToDouble__(retrivedValue));
  } else if (isInteger__(retrivedValue)) {
    resultValue.setValue_(deserializeStringToInt__(retrivedValue));
  } else {
    resultValue.setValue_(retrivedValue);
  }

  database__->Close();
  checkStatus__(status);

  return resultValue;
}

void RocksDBDataRepository::deleteData_(const std::string &key, const std::string &db) {
  setupDatabase__(db);
  rocksdb::Status status = database__->Delete(rocksdb::WriteOptions(), key);

  database__->Close();
  checkStatus__(status);
}
