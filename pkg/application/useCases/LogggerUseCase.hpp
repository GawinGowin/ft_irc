#ifndef LOGGGERUSECASE_HPP
#define LOGGGERUSECASE_HPP

#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include <string>

class LogggerUseCase {
public:
  LogggerUseCase(int type, std::string logFile = "") { LoggerServiceLocator::init(type, logFile); }
  ~LogggerUseCase() { LoggerServiceLocator::clean(); }

private:
  LogggerUseCase(void);
  LogggerUseCase(const LogggerUseCase &);
  LogggerUseCase &operator=(const LogggerUseCase &);
};

#endif /* LOGGGERUSECASE_HPP */
