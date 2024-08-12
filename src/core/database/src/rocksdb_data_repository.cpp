#include "../include/rocksdb_data_repository.hpp"
#include "../../../utils/include/constants.hpp"
#include "../../../utils/include/logs/logger.hpp"
#include "../../../utils/include/json/simple_json_parser.hpp"
#include <rocksdb/iterator.h>

bool RocksDBDataRepository::isActive_() {
  return database__ != nullptr;
}

bool RocksDBDataRepository::isFieldIndexed__(const std::string &fieldName) const {
  for (const auto &pair : fields__) {
    if (pair.first == fieldName) return true;
  }
  return false;
}

void RocksDBDataRepository::setDatabaseName__(const std::string &name) {
  databaseName__ = name;
}

void RocksDBDataRepository::setCurrentFilePath__(const std::string &path) {
  currentFilePath__ = path;
}

void RocksDBDataRepository::closeDatabaseConnection_() {
  if (isActive_()) {
    database__->Close();
  }
}

void RocksDBDataRepository::checkStatus__(const rocksdb::Status &status) {
  if (status.ok()) return;
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

void RocksDBDataRepository::setupDatabase__(const std::string &db,
                                            bool createDb) {
  std::string filePath = db;

  rocksdb::Options options;
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();

  if (createDb) {
    options.create_if_missing = true;
  }

  if (filePath.find(std::string(Constants::DEFAULT_DATABASE_DIRECTORY)) == std::string::npos) {
    filePath = std::string(Constants::DEFAULT_DATABASE_DIRECTORY) + filePath;
  }

  if (filePath != currentFilePath__) {
    closeDatabaseConnection_();
    rocksdb::Status status = rocksdb::DB::Open(options, filePath, &database__);

    checkStatus__(status);
    setCurrentFilePath__(filePath);
  }
  if (db.find('/') == std::string::npos) {
    setDatabaseName__(filePath);
  }
}

std::string RocksDBDataRepository::addDataHelper__(const QueryParserValue &value,
                                                   const std::string &key) {
  std::string valueString;
  SimpleJsonParser jsonParser;

  if (value.isString_()) {
    valueString = value.getString_();

  } else if (value.isInt_()) {
    long int valueToStore = value.getInt_();
    valueString = std::to_string(valueToStore);

  } else if (value.isDouble_()) {
    long double valueToStore = value.getDouble_();

    std::ostringstream oss;

    oss.precision(10);
    oss << valueToStore;

    valueString = oss.str();

  } else if (value.isArray_()) {
    JsonDataBuilder::JsonGenericParamsVector valuesToStore;

    for (const auto &val : value.getArray_()) {
      if (val.isString_()) {
        valuesToStore.push_back(val.getString_());
      } else if (val.isInt_()) {
        valuesToStore.push_back(val.getInt_());
      } else if (val.isDouble_()) {
        valuesToStore.push_back(val.getDouble_());
      }
    }

    valueString = jsonParser.encodeToJsonString_(jsonParser.jsonBuilder_->set_(key, valuesToStore));
  }
  return valueString;
}

std::string RocksDBDataRepository::getAndChangeFiledIndex__(const std::string &key) {
  size_t resultValue;
  for (auto &pair : fields__) {
    if (pair.first == key) {
      pair.second++;
      resultValue = pair.second;
      break;
    }
  }
  return std::to_string(resultValue);
}

void RocksDBDataRepository::addData_(const std::string &key,
                                     const QueryParserValue &value,
                                     const std::string &db,
                                     const DataType &dataType) {
  setupDatabase__(db);
  rocksdb::Status status;
  
  if (dataType == DataType::JSON) {
    std::string keyToStore = key;
    std::string dataToStore;

    bool flag = isInteger__(key);

    if (!flag) {

      keyToStore += "_";

      if (isFieldIndexed__(key)) {
        keyToStore += getAndChangeFiledIndex__(key);
      } else {
        fields__.push_back({key, 1});
        keyToStore += "1";
      }
    } 

    dataToStore = addDataHelper__(value, keyToStore);

    status = database__->Put(rocksdb::WriteOptions(), keyToStore, dataToStore);

    if (!flag) {
      notify_(*this, keyToStore);
    }
  } else {
    status = database__->Put(rocksdb::WriteOptions(), key, value.getString_());
  }
  checkStatus__(status);
}

RocksDBDataRepository::AllDataMap RocksDBDataRepository::getAllKeysAndValues_(const std::string &db) {
  setupDatabase__(db);
  AllDataMap result;

  rocksdb::Iterator *it = database__->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    result[it->key().ToString()] = it->value().ToString();
  }

  checkStatus__(it->status());
  return result;
}

QueryParserValue RocksDBDataRepository::getData_(const std::string &key, const std::string &db) {
  setupDatabase__(db, false);

  std::string retrivedValue;
  rocksdb::Status status = database__->Get(rocksdb::ReadOptions(), key, &retrivedValue);

  checkStatus__(status);
  return {retrivedValue};
}

void RocksDBDataRepository::deleteData_(const std::string &key, const std::string &db) {
  setupDatabase__(db, false);
  rocksdb::Status status = database__->Delete(rocksdb::WriteOptions(), key);
  checkStatus__(status);
}

RocksDBDataRepository::~RocksDBDataRepository() {
  closeDatabaseConnection_();
}
