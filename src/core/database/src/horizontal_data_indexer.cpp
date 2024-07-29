#include "../include/horizontal_data_indexer.hpp"

std::pair<std::string, std::string> HorizontalDataIndexer::getKeyAndValueFromArg__(const std::string &arg) {
  size_t semiColonPos = arg.rfind("_");

  if (semiColonPos == std::string::npos) throw std::runtime_error("Error");
  std::string key, value;
  value = arg.substr(0, semiColonPos);
  key = arg.substr(semiColonPos + 1, arg.length());

  return {key, value}; // {"id" : "value"}
}

bool HorizontalDataIndexer::checkIfValidPath__(const std::string &path) const {
  std::string mainDatabaseName = std::string(Constants::DEFAULT_DATABASE_DIRECTORY);

  if (path.find(mainDatabaseName) == std::string::npos) return false;

  std::string remainingPath = path.substr(mainDatabaseName.length());
  if (remainingPath.empty() || remainingPath == "/") return true;

  int count = std::count(remainingPath.begin(), remainingPath.end(), '/');
  if (remainingPath.back() == '/') {
    count--;
  }

  return count < Constants::ALLOWED_NESTED_FILES;
}

void HorizontalDataIndexer::dataChanged_(DataRepository &source, const std::string &args) {
  std::string tempArgs = args;

  auto argsData = getKeyAndValueFromArg__(tempArgs);
  std::string value {argsData.second}, id {argsData.first};

  std::string dbToStore = source.getDatabaseName_() + "/" + value;
  if (!checkIfValidPath__(dbToStore)) return ;

  auto valueToStore = source.getData_(tempArgs, source.getDatabaseName_());

  source.addData_(id, valueToStore, dbToStore);
  source.deleteData_(tempArgs, source.getDatabaseName_());
}
