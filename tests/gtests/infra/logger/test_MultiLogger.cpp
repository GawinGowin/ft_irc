#include "infra/logger/ILogger.hpp"
#include "infra/logger/MultiLogger.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockLogger : public ILogger {
public:
  MOCK_METHOD(void, trace, (std::string msg));
  MOCK_METHOD(void, debug, (std::string msg));
  MOCK_METHOD(void, info, (std::string msg));
  MOCK_METHOD(void, warning, (std::string msg));
  MOCK_METHOD(void, error, (std::string msg));
  MOCK_METHOD(void, fatal, (std::string msg));
  MOCK_METHOD(void, log, (LogLevel level, const std::string &msg));
};

class MultiLoggerTest : public ::testing::Test {};

TEST_F(MultiLoggerTest, TraceLevel) {
  MultiLogger logger;
  auto mockLogger = new MockLogger();
  logger.addLogger(mockLogger);

  EXPECT_CALL(*mockLogger, trace("Trace message")).Times(1);
  logger.trace("Trace message");
}

TEST_F(MultiLoggerTest, DebugLevel) {
  MultiLogger logger;
  auto mockLogger = new MockLogger();
  logger.addLogger(mockLogger);

  EXPECT_CALL(*mockLogger, debug("Debug message")).Times(1);
  logger.debug("Debug message");
}

TEST_F(MultiLoggerTest, DualLoggers) {
  MultiLogger logger;
  auto mockLogger1 = new MockLogger();
  auto mockLogger2 = new MockLogger();
  logger.addLogger(mockLogger1);
  logger.addLogger(mockLogger2);

  EXPECT_CALL(*mockLogger1, info("Info message")).Times(1);
  EXPECT_CALL(*mockLogger2, info("Info message")).Times(1);
  logger.info("Info message");
}

TEST_F(MultiLoggerTest, MultipleFatalLoggers) {
  MultiLogger logger;
  auto mockLogger1 = new MockLogger();
  auto mockLogger2 = new MockLogger();
  logger.addLogger(mockLogger1);
  logger.addLogger(mockLogger2);

  EXPECT_CALL(*mockLogger1, fatal("Fatal message"))
      .WillOnce(testing::Throw(std::runtime_error("Fatal message1")));
  EXPECT_CALL(*mockLogger2, fatal("Fatal message"))
      .WillOnce(testing::Throw(std::runtime_error("Fatal message2")));
  EXPECT_THROW(logger.fatal("Fatal message"), std::runtime_error);
}

TEST_F(MultiLoggerTest, WithoutLoggers) {
  MultiLogger logger;
  EXPECT_NO_THROW(logger.trace("Trace message"));
  EXPECT_NO_THROW(logger.info("Info message"));
  EXPECT_NO_THROW(logger.debug("Debug message"));
  EXPECT_NO_THROW(logger.warning("Warning message"));
  EXPECT_NO_THROW(logger.error("Error message"));
  EXPECT_THROW(logger.fatal("Fatal message"), std::runtime_error);
}
