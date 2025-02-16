#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP

#include "infra/logger/ILogger.hpp"
#include "infra/logger/ILoggerStream.hpp"

#include <ctime>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

class FileLogger : virtual public ILogger, virtual public ILoggerStream {
public:
  FileLogger(std::string logFile) {
    this->_ofile.open(logFile.c_str(), std::ios::out | std::ios::app);
    if (!this->_ofile.is_open()) {
      throw std::runtime_error("Failed to open log file: " + logFile);
    }
  }

  ~FileLogger() {
    if (this->_ofile.is_open()) {
      this->_ofile.close();
    }
  }

  void trace(std::string msg) {
    if (this->_ofile.is_open()) {
      this->_ofile << this->getTime() << " [trace]: " << msg << std::endl;
    }
  }

  void debug(std::string msg) {
    if (this->_ofile.is_open()) {
      this->_ofile << this->getTime() << " [debug]: " << msg << std::endl;
    }
  }

  void info(std::string msg) {
    if (this->_ofile.is_open()) {
      this->_ofile << this->getTime() << " [info]: " << msg << std::endl;
    }
  }

  void warning(std::string msg) {
    if (this->_ofile.is_open()) {
      this->_ofile << this->getTime() << " [warning]: " << msg << std::endl;
    }
  }

  void error(std::string msg) {
    if (this->_ofile.is_open()) {
      this->_ofile << this->getTime() << " [error]: " << msg << std::endl;
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

private:
  std::fstream _ofile;

  std::string getTime(void) {
    std::stringstream ss;
    std::time_t now = std::time(NULL);
    std::tm *localTime = std::localtime(&now);

    ss << (localTime->tm_year + 1900) << "-" << (localTime->tm_mon + 1) << "-" << localTime->tm_mday
       << " " << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec;
    return ss.str();
  }

  FileLogger(const FileLogger &other);
  FileLogger &operator=(const FileLogger &other);
  FileLogger();
};

#endif /* FILELOGGER_HPP */
