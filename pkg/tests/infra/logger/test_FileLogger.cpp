#include "infra/logger/FileLogger.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

class FileLoggerTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Remove any existing log files before each test
    std::remove("test_trace.log");
    std::remove("test_debug.log");
    std::remove("test_info.log");
    std::remove("test_warning.log");
    std::remove("test_error.log");
  }

  void TearDown() override {
    // Clean up log files after each test
    std::remove("test_trace.log");
    std::remove("test_debug.log");
    std::remove("test_info.log");
    std::remove("test_warning.log");
    std::remove("test_error.log");
  }
};

TEST_F(FileLoggerTest, TraceLog) {
  std::string logFile = "test_trace.log";
  FileLogger logger(logFile);
  logger.trace("This is a trace message");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());

  std::string line;
  std::getline(ifile, line);
  ASSERT_NE(line.find("[trace]: This is a trace message"), std::string::npos);

  ifile.close();
}

TEST_F(FileLoggerTest, DebugLog) {
  std::string logFile = "test_debug.log";
  FileLogger logger(logFile);
  logger.debug("This is a debug message");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());

  std::string line;
  std::getline(ifile, line);
  ASSERT_NE(line.find("[debug]: This is a debug message"), std::string::npos);

  ifile.close();
}

TEST_F(FileLoggerTest, InfoLog) {
  std::string logFile = "test_info.log";
  FileLogger logger(logFile);
  logger.info("This is an info message");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());

  std::string line;
  std::getline(ifile, line);
  ASSERT_NE(line.find("[info]: This is an info message"), std::string::npos);

  ifile.close();
}

TEST_F(FileLoggerTest, WarningLog) {
  std::string logFile = "test_warning.log";
  FileLogger logger(logFile);
  logger.warning("This is a warning message");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());

  std::string line;
  std::getline(ifile, line);
  ASSERT_NE(line.find("[warning]: This is a warning message"), std::string::npos);

  ifile.close();
}

TEST_F(FileLoggerTest, ErrorLog) {
  std::string logFile = "test_error.log";
  FileLogger logger(logFile);
  logger.error("This is an error message");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());

  std::string line;
  std::getline(ifile, line);
  ASSERT_NE(line.find("[error]: This is an error message"), std::string::npos);

  ifile.close();
}

TEST_F(FileLoggerTest, MultipleMessages) {
  std::string logFile = "test_multiple.log";
  FileLogger logger(logFile);
  logger.trace("Trace message");
  logger.debug("Debug message");
  logger.info("Info message");
  logger.warning("Warning message");
  logger.error("Error message");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());

  std::string line;
  std::getline(ifile, line);
  ASSERT_NE(line.find("[trace]: Trace message"), std::string::npos);
  std::getline(ifile, line);
  ASSERT_NE(line.find("[debug]: Debug message"), std::string::npos);
  std::getline(ifile, line);
  ASSERT_NE(line.find("[info]: Info message"), std::string::npos);
  std::getline(ifile, line);
  ASSERT_NE(line.find("[warning]: Warning message"), std::string::npos);
  std::getline(ifile, line);
  ASSERT_NE(line.find("[error]: Error message"), std::string::npos);

  ifile.close();
  std::remove(logFile.c_str());
}
