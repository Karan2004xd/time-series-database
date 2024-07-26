#pragma once

#include <mutex>
#include <sstream>
#include <utility>

class Logger {
private:
  static void initLogging__();
  void logMessage__();
  Logger();

public:
  Logger(const Logger &) = delete;
  void operator=(const Logger &) = delete;

  enum SeverityType {
    INFO, ERROR, WARNING
  };

private:
  SeverityType currentSeverity__;
  std::stringstream buffer__;
  std::mutex logMutex__;

public:
  static Logger &log_(const SeverityType &severityType);

  // Make sure to add a new line at the end of the message or else some unexpected beheviour can happen
  template <typename ValueType>
  Logger &operator<<(ValueType &&message) {
    buffer__ << std::forward<ValueType>(message);
    if (buffer__.str().back() == '\n') {
      logMessage__();
    }
    return *this;
  }

  ~Logger();
};
