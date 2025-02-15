#ifndef MULTILOGGER_HPP
#define MULTILOGGER_HPP

#include "ILogger.hpp"
#include <vector>

class MultiLogger : public ILogger {
public:
  MultiLogger(): _loggers(std::vector<ILogger *>()) {}
  ~MultiLogger() {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      if (*it != NULL)
        delete *it;
    }
  }

  void addLogger(ILogger *logger) { _loggers.push_back(logger); }

  void trace(std::string msg) {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      (*it)->trace(msg);
    }
  }

  void debug(std::string msg) {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      (*it)->debug(msg);
    }
  }

  void info(std::string msg) {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      (*it)->info(msg);
    }
  }

  void warning(std::string msg) {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      (*it)->warning(msg);
    }
  }

  void error(std::string msg) {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      (*it)->error(msg);
    }
  }

private:
  MultiLogger(const MultiLogger &other);
  MultiLogger &operator=(const MultiLogger &other);
  std::vector<ILogger *> _loggers;
};

#endif /* MULTILOGGER_HPP */
