#ifndef LOGGERSERVICELOCATOR_HPP
#define LOGGERSERVICELOCATOR_HPP

#include "infra/logger/ConsoleLogger.hpp"
#include "infra/logger/FileLogger.hpp"
#include "infra/logger/ILogger.hpp"
#include "infra/logger/MultiLogger.hpp"

#if defined(USE_SENTRY)
#include "infra/logger/SentryLogger.hpp"
#endif

#include <stdexcept>
#include <string>

class LoggerServiceLocator {
public:
  enum loggerType { CONSOLE = 1 << 0, FILE = 1 << 1, SENTRY = 1 << 2 };

  static void init(int type, std::string logFile = "") {
    if (_logger != NULL) {
      throw std::runtime_error("LoggerServiceLocator is already initialized");
    }
    _logger = new MultiLogger();
    if (type & CONSOLE) {
      _logger->addLogger(new ConsoleLogger());
    }
    if (type & FILE) {
      if (logFile.empty()) {
        throw std::runtime_error("logFile is empty");
      }
      _logger->addLogger(new FileLogger(logFile));
    }
#if defined(USE_SENTRY)
    if (type & SENTRY) {
      _logger->addLogger(new SentryLogger());
    }
#endif
    _logger->trace("Logger initialized");
  }

  static MultiLogger *get() {
    if (_logger == NULL) {
      throw std::runtime_error("LoggerServiceLocator is not initialized");
    }
    return _logger;
  }

  static void clean() {
    _logger->trace("Logger cleaned");
    if (_logger != NULL) {
      delete _logger;
      _logger = NULL;
    }
  }

private:
  static MultiLogger *_logger;
};

#endif /* LOGGERSERVICELOCATOR_HPP */
