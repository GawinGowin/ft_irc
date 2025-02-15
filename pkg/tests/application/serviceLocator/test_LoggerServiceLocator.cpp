#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include <gtest/gtest.h>

TEST(LoggerServiceLocatorTest, ConsoleLogger) {
  LoggerServiceLocator::init(LoggerServiceLocator::CONSOLE);
  MultiLogger &logger = LoggerServiceLocator::get();
  testing::internal::CaptureStdout();
  logger.trace("Trace message");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\e[30m[trace]: Trace message\e[m\n");
}
