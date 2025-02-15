#ifndef LOGGERSERVICELOCATOR_HPP
#define LOGGERSERVICELOCATOR_HPP

#include "infra/logger/ConsoleLogger.hpp"
#include "infra/logger/FileLogger.hpp" 
#include "infra/logger/ILogger.hpp"
#include "infra/logger/MultiLogger.hpp"
#include <string>
#include <stdexcept>

class LoggerServiceLocator {
public:
  enum loggerType {
    CONSOLE = 1 << 0,
    FILE = 1 << 1,
  };

  static void init(int type, std::string logFile = "") {
    if (_logger.size() > 0) {
      _logger.clear();
    }
    if (type & CONSOLE) {
      _logger.addLogger(new ConsoleLogger());
    }
    if (type & FILE) {
      if (logFile.empty()) {
        throw std::runtime_error("logFile is empty");
      }
      _logger.addLogger(new FileLogger(logFile));
    }
  }

  static MultiLogger &get() { return _logger; }

private:
  static MultiLogger _logger;
};

#endif /* LOGGERSERVICELOCATOR_HPP */
