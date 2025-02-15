#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

class ILogger {
public:
  virtual ~ILogger(){};
  virtual void trace(std::string msg) = 0;
  virtual void debug(std::string msg) = 0;
  virtual void info(std::string msg) = 0;
  virtual void warning(std::string msg) = 0;
  virtual void error(std::string msg) = 0;
};

#endif /* ILOGGER_HPP */
