#include "infra/logger/ConsoleLogger.hpp"
#include <gtest/gtest.h>

class ConsoleLoggerTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ConsoleLoggerTest, TraceLevel) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.trace("Trace message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[30m[trace]: Trace message\e[m\n");
}

TEST_F(ConsoleLoggerTest, DebugLevel) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.debug("Debug message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "[debug]: Debug message\e[m\n");
}

TEST_F(ConsoleLoggerTest, InfoLevel) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.info("Info message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[35m[info]: Info message\e[m\n");
}

TEST_F(ConsoleLoggerTest, WarningLevel) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.warning("Warning message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[33m[warning]: Warning message\e[m\n");
}

TEST_F(ConsoleLoggerTest, ErrorLevel) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.error("Error message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[31m[error]: Error message\e[m\n");
}

TEST_F(ConsoleLoggerTest, TraceLevelSS) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.tracess() << "Trace message";
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[30m[trace]: Trace message\e[m\n");
}

TEST_F(ConsoleLoggerTest, DebugLevelSS) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.debugss() << "Debug message";
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "[debug]: Debug message\e[m\n");
}

TEST_F(ConsoleLoggerTest, InfoLevelSS) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.infoss() << "Info message";
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[35m[info]: Info message\e[m\n");
}

TEST_F(ConsoleLoggerTest, WarningLevelSS) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.warningss() << "Warning message";
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[33m[warning]: Warning message\e[m\n");
}

TEST_F(ConsoleLoggerTest, ErrorLevelSS) {
  ConsoleLogger logger;
  testing::internal::CaptureStdout();
  logger.errorss() << "Error message";
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[31m[error]: Error message\e[m\n");
}
