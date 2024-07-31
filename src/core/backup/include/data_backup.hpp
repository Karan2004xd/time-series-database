#pragma once
#include "../../../core/database/include/data_repository.hpp"

struct DataBackup : public Observer<DataRepository> {
  // list of json encoded fields of data will be provided
  // {fields : ["field_1", "field_2", ....]}
  void dataChanged_(DataRepository &source, const std::string &args) override;
};
