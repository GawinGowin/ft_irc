#ifndef LOGGGERWRAPPER_HPP
#define LOGGGERWRAPPER_HPP

#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include <string>

class LogggerWrapper {
public:
  LogggerWrapper(int type, std::string logFile = "") { LoggerServiceLocator::init(type, logFile); }
  ~LogggerWrapper() { LoggerServiceLocator::clean(); }

private:
  LogggerWrapper(void);
  LogggerWrapper(const LogggerWrapper &);
  LogggerWrapper &operator=(const LogggerWrapper &);
};

#endif /* LOGGGERWRAPPER_HPP */
