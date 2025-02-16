#ifndef MULTILOGGER_HPP
#define MULTILOGGER_HPP

#include "ILogger.hpp"
#include <vector>

class MultiLogger : virtual public ILogger {
public:
  MultiLogger() : _loggers(std::vector<ILogger *>()) {}
  ~MultiLogger() { this->clear(); }
  MultiLogger(const MultiLogger &other) { _loggers = other._loggers; }
  MultiLogger &operator=(const MultiLogger &other) {
    if (this != &other) {
      this->clear();
      _loggers = other._loggers;
    }
    return *this;
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

  std::size_t size() { return _loggers.size(); }

  void clear() {
    std::vector<ILogger *>::iterator it;
    for (it = _loggers.begin(); it != _loggers.end(); it++) {
      if (*it != NULL) {
        delete *it;
        *it = NULL;
      }
    }
    _loggers.clear();
  }

private:
  std::vector<ILogger *> _loggers;
};

#endif /* MULTILOGGER_HPP */
