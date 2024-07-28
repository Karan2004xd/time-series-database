#pragma once
#include "../../observer/include/observer.hpp"
#include "data_repository.hpp"

struct DataIndexer : public Observer<DataRepository> {
  virtual void indexData() = 0;
  void dataChanged_(DataRepository &source, const std::string &args) override;
};
