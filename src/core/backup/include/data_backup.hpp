#pragma once
#include "../../../core/database/include/data_repository.hpp"

struct DataBackup : public Observer<DataRepository> {
  // {field : <field name>, key : <key of the indexed data>, value : <data>}
  virtual void dataChanged_(DataRepository &source, const std::string &args) override = 0;
};
