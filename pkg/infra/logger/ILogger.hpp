#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <sstream>
#include <string>

#include "infra/logger/LogLevel.hpp"

class ILogger {
public:
  virtual ~ILogger(){};
  virtual void trace(std::string msg) = 0;
  virtual void debug(std::string msg) = 0;
  virtual void info(std::string msg) = 0;
  virtual void warning(std::string msg) = 0;
  virtual void error(std::string msg) = 0;

  virtual void log(LogLevel level, const std::string &msg) {
    switch (level) {
    case TRACE:
      this->trace(msg);
      break;
    case DEBUG:
      this->debug(msg);
      break;
    case INFO:
      this->info(msg);
      break;
    case WARNING:
      this->warning(msg);
      break;
    case ERROR:
      this->error(msg);
      break;
    }
  }
};

#endif /* ILOGGER_HPP */
