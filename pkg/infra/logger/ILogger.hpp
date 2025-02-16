#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <sstream>
#include <string>

#include "infra/logger/LogStream.hpp"
#include "infra/logger/LogLevel.hpp"

class ILogger {
public:
  virtual ~ILogger(){};
  virtual void trace(std::string msg) = 0;
  virtual void debug(std::string msg) = 0;
  virtual void info(std::string msg) = 0;
  virtual void warning(std::string msg) = 0;
  virtual void error(std::string msg) = 0;

  virtual void log(LogLevel level, const std::string &msg) = 0;

  virtual LogStream tracess(void) = 0;
  virtual LogStream debugss(void) = 0;
  virtual LogStream infoss(void) = 0;
  virtual LogStream warningss(void) = 0;
  virtual LogStream errorss(void) = 0;
};

#endif /* ILOGGER_HPP */
