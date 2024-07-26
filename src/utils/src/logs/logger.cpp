#include "../../include/logs/logger.hpp"
#include "../../include/constants.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <filesystem>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void Logger::initLogging__() {
  std::string filePath = std::filesystem::current_path().parent_path();
  std::string fileName = filePath + "/"
    + std::string(Constants::MAIN_SYSTEM_LOG_DIRECTORY_NAME) + "/"
    + std::string(Constants::LOG_FILE_NAME);

  logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

  logging::add_file_log(
    keywords::file_name = fileName,
    keywords::rotation_size = Constants::LOG_ROTATION_SIZE,
    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
    keywords::format = Constants::LOG_FORMAT
    /* keywords::open_mode = std::ios_base::app */
  );


  logging::core::get()->set_filter(
    logging::trivial::severity >= logging::trivial::info
  );

  logging::add_common_attributes();
}

void Logger::logMessage__() {
  std::lock_guard<std::mutex> lock {logMutex__};
  switch (currentSeverity__) {
    case INFO:
      BOOST_LOG_TRIVIAL(info) << buffer__.str();
      break;
    case WARNING:
      BOOST_LOG_TRIVIAL(warning) << buffer__.str();
      break;
    case ERROR:
      BOOST_LOG_TRIVIAL(error) << buffer__.str();
      break;
  }
  buffer__.str("");
  buffer__.clear();
}

Logger::Logger() {
  initLogging__();
}

Logger &Logger::log_(const SeverityType &severityType) {
  static Logger logger;
  logger.currentSeverity__ = severityType;
  return logger;
}

Logger::~Logger() {
  if (!buffer__.str().empty()) {
    logMessage__();
  }
}
