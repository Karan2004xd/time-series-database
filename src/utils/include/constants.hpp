#include <string_view>

struct Constants {
  typedef std::string_view String;

  // Logger class constants
  static constexpr String LOG_FILE_NAME = "activity_%N.log";
  static constexpr int LOG_ROTATION_SIZE = 10 * 1024 * 1024; // 10MB
  static constexpr String LOG_FORMAT = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%";
  static constexpr String MAIN_SYSTEM_LOG_DIRECTORY_NAME = "system_logs";

  // database constants
  static constexpr String DEFAULT_DATABASE_DIRECTORY = "/tmp/";
  static constexpr String METADATA_DATABASE_NAME = "metadata";
  static constexpr String METADATA_FIELDS_DATASET_NAME = "fields";
  static constexpr String FIELDS_JSON_FORMAT_KEY_NAME = "names";
  static constexpr int ALLOWED_NESTED_FILES = 2;

  // backup constansts
  static constexpr String DEFAULT_S3_BUCKET_REGION = "eu-north-1";
  static constexpr String S3_TEST_BUCKET = "eu-north-1-test-bucket";
};
