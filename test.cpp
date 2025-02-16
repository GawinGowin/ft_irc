#include <iostream>
#include <sstream>
#include <string>

class ILogger {
public:
  virtual ~ILogger() {}
  virtual void log(const std::string &level, const std::string &msg) = 0;
};

class LogStream {
private:
  ILogger &logger;
  std::string level;
  std::ostringstream stream;

public:
  LogStream(ILogger &logger, const std::string &level) : logger(logger), level(level) {}

  template <typename T> LogStream &operator<<(const T &value) {
    stream << value;
    return *this;
  }
  LogStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    stream << manip;
    return *this;
  }

  ~LogStream() { logger.log(level, stream.str()); }
  LogStream(const LogStream &other)
      : logger(other.logger), level(other.level), stream(other.stream.str()) {}
};

// ConsoleLogger クラス
class ConsoleLogger : public ILogger {
public:
  void log(const std::string &level, const std::string &msg) {
    if (level == "trace")
      std::cout << "\e[30m[trace]: " << msg << "\e[m" << std::endl;
    else if (level == "debug")
      std::cout << "[debug]: " << msg << "\e[m" << std::endl;
    else if (level == "info")
      std::cout << "\e[35m[info]: " << msg << "\e[m" << std::endl;
    else if (level == "warning")
      std::cout << "\e[33m[warning]: " << msg << "\e[m" << std::endl;
    else if (level == "error")
      std::cout << "\e[31m[error]: " << msg << "\e[m" << std::endl;
  }

  // ログレベルごとのストリームを返す関数
  LogStream trace() { return LogStream(*this, "trace"); }
  LogStream debug() { return LogStream(*this, "debug"); }
  LogStream info() { return LogStream(*this, "info"); }
  LogStream warning() { return LogStream(*this, "warning"); }
  LogStream error() { return LogStream(*this, "error"); }
};

int main() {
  ConsoleLogger logger;
  logger.trace() << "Operation start " << 42 << "!";

  logger.debug() << "Debugging value: " << 3.14;
  logger.info() << "User logged in: "
                << "JohnDoe";
  logger.warning() << "Low disk space: " << 5 << "% remaining";
  logger.error() << "Fatal error: "
                 << "Segmentation fault";
  return 0;
}
