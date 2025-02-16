#ifndef STREAMLOGGER_HPP
#define STREAMLOGGER_HPP

#include "infra/logger/LogStream.hpp"
#include "infra/logger/ILogger.hpp"
#include "infra/logger/LogLevel.hpp"

class StreamLogger {
public:
  StreamLogger(ILogger *logger): _logger(logger) {}
  ~StreamLogger() {}
  LogStream tracess(void) { return LogStream(_logger, TRACE); }
  LogStream debugss(void) { return LogStream(_logger, DEBUG); }
  LogStream infoss(void) { return LogStream(_logger, INFO); }
  LogStream warningss(void) { return LogStream(_logger, WARNING); }
  LogStream errorss(void) { return LogStream(_logger, ERROR); }

private:
  ILogger *_logger;
};

#endif /* STREAMLOGGER_HPP */
