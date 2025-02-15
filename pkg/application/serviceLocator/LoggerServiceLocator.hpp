#ifndef LOGGERSERVICELOCATOR_HPP
#define LOGGERSERVICELOCATOR_HPP

#include "infra/logger/ConsoleLogger.hpp"
#include "infra/logger/ILogger.hpp"
#include "infra/logger/MultiLogger.hpp"

class LoggerServiceLocator {
public:
  enum loggerType {
    CONSOLE = 1 << 0,
  };

  static void init(int type) {
    if (type & CONSOLE) {
      _logger.addLogger(new ConsoleLogger());
    }
  }

  static MultiLogger &get() { return _logger; }

private:
  static MultiLogger _logger;
};

#endif /* LOGGERSERVICELOCATOR_HPP */
