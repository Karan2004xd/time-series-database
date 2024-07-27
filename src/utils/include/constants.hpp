#include <string_view>

struct Constants {
  typedef std::string_view String;

  // Logger class constants
  static constexpr String LOG_FILE_NAME = "activity_%N.log";
  static constexpr int LOG_ROTATION_SIZE = 10 * 1024 * 1024; // 10MB
  static constexpr String LOG_FORMAT = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%";
  static constexpr String MAIN_SYSTEM_LOG_DIRECTORY_NAME = "system_logs";

  // database constants
  static constexpr String DEFAULT_DIRECTORY = "/tmp/";
};
