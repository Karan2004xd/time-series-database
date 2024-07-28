#pragma once

#include "data_indexer.hpp"

class HorizontalDataIndexer : public DataIndexer {
public:
  void dataChanged_(DataRepository &source, const std::string &args) override;
  void indexData() override;
};
