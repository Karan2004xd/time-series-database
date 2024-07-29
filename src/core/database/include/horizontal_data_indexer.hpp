#pragma once

#include "../../observer/include/observer.hpp"
#include "../../../utils/include/constants.hpp"
#include "data_repository.hpp"
#include <rocksdb/db.h>

class HorizontalDataIndexer : public Observer<DataRepository> {
private:
  std::pair<std::string, std::string> getKeyAndValueFromArg__(const std::string &arg);
  bool checkIfValidPath__(const std::string &path) const;

public:
  // expecting database name in args (only name no path)
  // expecting the data of metadata to be in format
  // fields : {"name" : "name of the new field"}
  void dataChanged_(DataRepository &source, const std::string &args) override;
};
