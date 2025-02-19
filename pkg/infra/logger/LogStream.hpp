#ifndef LOGSTREAM_HPP
#define LOGSTREAM_HPP

#include "infra/logger/ILogger.hpp"
#include "infra/logger/LogLevel.hpp"

enum LogLevel;

class LogStream {
private:
  ILogger *_logger;
  LogLevel _level;
  std::ostringstream _stream;

public:
  LogStream(ILogger *logger, LogLevel level) : _logger(logger), _level(level) {}

  template <typename T> LogStream &operator<<(const T &value) {
    _stream << value;
    return *this;
  }
  LogStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    _stream << manip;
    return *this;
  }

  ~LogStream() { this->_logger->log(this->_level, this->_stream.str()); }
  LogStream(const LogStream &other)
      : _logger(other._logger), _level(other._level), _stream(other._stream.str()) {}
};

#endif /* LOGSTREAM_HPP */
