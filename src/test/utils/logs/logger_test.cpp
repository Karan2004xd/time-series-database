#include "../../../utils/include/logs/logger.hpp"
#include "../../../utils/include/constants.hpp"

#include <gtest/gtest.h>
#include <filesystem>

std::string filePath = std::filesystem::current_path().parent_path().parent_path().parent_path();
std::string fileName = filePath + "/"
  + std::string(Constants::MAIN_SYSTEM_LOG_DIRECTORY_NAME);

TEST(LoggerTestSuite, CheckIfLoggingDirectoryExists) {
  ASSERT_TRUE(std::filesystem::exists(fileName) && std::filesystem::is_directory(fileName));
}

TEST(LoggerTestSuite, SingletonPropertyCheck) {
  Logger &loggerOne = Logger::log_(Logger::INFO);
  Logger &loggerTwo = Logger::log_(Logger::INFO);

  ASSERT_EQ(&loggerOne, &loggerTwo);
}

TEST(LoggerTestSuite, LoggingTest) {
  EXPECT_NO_THROW({
    Logger::log_(Logger::INFO) << "Hello world\n";
    Logger::log_(Logger::WARNING) << "Hello world" << ", 5 + 5 = " << 5 + 5 << "\n";
    Logger::log_(Logger::ERROR) << "Hello world again!" << "\n";
  });
}
