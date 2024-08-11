#pragma once
#include "../data_backup.hpp"
#include <atomic>

class AWSDataBackup : public DataBackup {
public:
  enum DurationType {
    SECONDS, MINUTES, HOURS, DAYS, MONTHS
  };

private:
  DurationType durationType__;
  size_t duration__;

  std::atomic<bool> stopBackupThread__;

protected: // for testing


public:
  AWSDataBackup();
  AWSDataBackup(const size_t &duration, const DurationType &durationType);

  void dataChanged_(DataRepository &source, const std::string &args) final override;
};
