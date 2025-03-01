#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP

#include "infra/logger/ILogger.hpp"
#include "infra/logger/StreamLogger.hpp"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

class FileLogger : virtual public ILogger, virtual public StreamLogger {
public:
  FileLogger(std::string logFile) : StreamLogger(this) {
    this->_ofile.open(logFile.c_str(), std::ios::out | std::ios::trunc);
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

  void fatal(std::string msg) {
    if (this->_ofile.is_open()) {
      this->_ofile << this->getTime() << " [fatal]: " << msg << std::endl;
    }
    throw std::runtime_error(msg);
  }

private:
  std::fstream _ofile;

  std::string getTime(void) {
    std::stringstream ss;
    std::time_t now = std::time(NULL);
    std::tm *localTime = std::localtime(&now);

    ss << (localTime->tm_year + 1900) << "-" << std::setw(2) << std::setfill('0')
       << (localTime->tm_mon + 1) << "-" << std::setw(2) << std::setfill('0') << localTime->tm_mday
       << " " << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":" << std::setw(2)
       << std::setfill('0') << localTime->tm_min << ":" << std::setw(2) << std::setfill('0')
       << localTime->tm_sec;
    return ss.str();
  }

  FileLogger(const FileLogger &other);
  FileLogger &operator=(const FileLogger &other);
  FileLogger();
};

#endif /* FILELOGGER_HPP */
