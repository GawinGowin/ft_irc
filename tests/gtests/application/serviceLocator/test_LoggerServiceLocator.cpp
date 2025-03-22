#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include <gtest/gtest.h>

const std::string logFile = "serviceLocator_test.log";

class LoggerServiceLocatorTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Remove any existing log files before each test
    std::remove(logFile.c_str());
  }
  void TearDown() override {
    LoggerServiceLocator::clean();
    std::remove(logFile.c_str());
  }
};

TEST_F(LoggerServiceLocatorTest, NoLogger) {
  LoggerServiceLocator::init(0);
  MultiLogger *logger = LoggerServiceLocator::get();
  testing::internal::CaptureStdout();
  logger->trace("Trace message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "");
}

TEST_F(LoggerServiceLocatorTest, ConsoleLogger) {
  LoggerServiceLocator::init(LoggerServiceLocator::CONSOLE);
  MultiLogger *logger = LoggerServiceLocator::get();
  testing::internal::CaptureStdout();
  logger->trace("Trace message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[30m[trace]: Trace message\e[m\n");
}

TEST_F(LoggerServiceLocatorTest, FileLogger) {
  LoggerServiceLocator::init(LoggerServiceLocator::FILE, logFile);
  MultiLogger *logger = LoggerServiceLocator::get();
  testing::internal::CaptureStdout();
  logger->trace("This is a trace message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());
  std::string fileContent(
      (std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
  ASSERT_NE(fileContent.find("[trace]: This is a trace message"), std::string::npos);
  ifile.close();
  std::remove(logFile.c_str());
}

TEST_F(LoggerServiceLocatorTest, ErrorFileLogger) {
  EXPECT_THROW(LoggerServiceLocator::init(LoggerServiceLocator::FILE), std::runtime_error);
}

TEST_F(LoggerServiceLocatorTest, MultiLogger) {
  LoggerServiceLocator::init(LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE, logFile);
  MultiLogger *logger = LoggerServiceLocator::get();
  testing::internal::CaptureStdout();
  logger->trace("Trace message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[30m[trace]: Trace message\e[m\n");

  std::ifstream ifile(logFile);
  ASSERT_TRUE(ifile.is_open());
  std::string fileContent(
      (std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
  ASSERT_NE(fileContent.find("[trace]: Trace message"), std::string::npos);
  ifile.close();
  std::remove(logFile.c_str());
}