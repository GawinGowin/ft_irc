#ifndef CONSOLELOGGER_HPP
#define CONSOLELOGGER_HPP

#include "infra/logger/ILogger.hpp"

#include <iostream>
#include <string>

class ConsoleLogger : virtual public ILogger {
public:
  void trace(std::string msg) { std::cout << "\e[30m[trace]: " << msg << "\e[m" << std::endl; }

  void debug(std::string msg) { std::cout << "[debug]: " << msg << "\e[m" << std::endl; }

  void info(std::string msg) { std::cout << "\e[35m[info]: " << msg << "\e[m" << std::endl; }

  void warning(std::string msg) { std::cout << "\e[33m[warning]: " << msg << "\e[m" << std::endl; }

  void error(std::string msg) { std::cout << "\e[31m[error]: " << msg << "\e[m" << std::endl; }

  void log(LogLevel level, const std::string &msg) {
    switch (level) {
    case TRACE:
      trace(msg);
      break;
    case DEBUG:
      debug(msg);
      break;
    case INFO:
      info(msg);
      break;
    case WARNING:
      warning(msg);
      break;
    case ERROR:
      error(msg);
      break;
    }
  }

  LogStream tracess(void) { return LogStream(this, TRACE); }

  LogStream debugss(void) { return LogStream(this, DEBUG); }

  LogStream infoss(void) { return LogStream(this, INFO); }

  LogStream warningss(void) { return LogStream(this, WARNING); }

  LogStream errorss(void) { return LogStream(this, ERROR); }
};

#endif /* CONSOLELOGGER_HPP */
