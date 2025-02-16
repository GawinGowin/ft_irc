#ifndef CONSOLELOGGER_HPP
#define CONSOLELOGGER_HPP

#include "ILogger.hpp"

#include <iostream>
#include <string>

class ConsoleLogger : virtual public ILogger {
public:
  void trace(std::string msg) { std::cout << "\e[30m[trace]: " << msg << "\e[m" << std::endl; }

  void debug(std::string msg) { std::cout << "[debug]: " << msg << "\e[m" << std::endl; }

  void info(std::string msg) { std::cout << "\e[35m[info]: " << msg << "\e[m" << std::endl; }

  void warning(std::string msg) { std::cout << "\e[33m[warning]: " << msg << "\e[m" << std::endl; }

  void error(std::string msg) { std::cout << "\e[31m[error]: " << msg << "\e[m" << std::endl; }
};

#endif /* CONSOLELOGGER_HPP */
