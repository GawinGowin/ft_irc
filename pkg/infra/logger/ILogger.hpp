#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <sstream>
#include <string>

#include "infra/logger/LogLevel.hpp"

/**
 * @class ILogger
 * @brief ログ出力のためのインターフェースクラス
 *
 * ILoggerは、異なるログレベルでメッセージを出力するための仮想関数を提供します。
 * このクラスを継承して、具体的なログ出力方法を実装します。
 *
 * @note このクラスはインターフェースとして機能するため、直接インスタンス化することはできません。
 */
class ILogger {
public:
  /**
 * @brief デストラクタ
 *
 * 派生クラスでリソースを解放するためにオーバーライドされることを想定しています。
 */
  virtual ~ILogger(){};
  /**
 * @brief トレースレベルのログメッセージを出力します。
 * @param msg ログメッセージ
 */
  virtual void trace(std::string msg) = 0;

  /**
 * @brief デバッグレベルのログメッセージを出力します。
 * @param msg ログメッセージ
 */
  virtual void debug(std::string msg) = 0;

  /**
 * @brief 情報レベルのログメッセージを出力します。
 * @param msg ログメッセージ
 */
  virtual void info(std::string msg) = 0;

  /**
 * @brief 警告レベルのログメッセージを出力します。
 * @param msg ログメッセージ
 */
  virtual void warning(std::string msg) = 0;

  /**
 * @brief エラーレベルのログメッセージを出力します。
 * @param msg ログメッセージ
 */
  virtual void error(std::string msg) = 0;

  /**
 * @brief 致命的なエラーレベルのログメッセージを出力し、std::runtime_errorをスローします。
 * @param msg ログメッセージ
 */
  virtual void fatal(std::string msg) = 0;

  /**
 * @brief 指定されたログレベルでメッセージを出力します。
 * @param level ログレベル
 * @param msg ログメッセージ
 *
 * @details ログレベルに応じて適切な仮想関数を呼び出します。
 */
  virtual void log(LogLevel level, const std::string &msg) {
    std::string sanitizedMsg = msg;
    if (!sanitizedMsg.empty()) {
      size_t pos = sanitizedMsg.find_last_not_of("\r\n");
      if (pos != std::string::npos) {
        sanitizedMsg.erase(pos + 1);
      }
    }
    switch (level) {
    case TRACE:
      this->trace(sanitizedMsg);
      break;
    case DEBUG:
      this->debug(sanitizedMsg);
      break;
    case INFO:
      this->info(sanitizedMsg);
      break;
    case WARNING:
      this->warning(sanitizedMsg);
      break;
    case ERROR:
      this->error(sanitizedMsg);
      break;
    case FATAL:
      this->fatal(sanitizedMsg);
      break;
    }
  }
};

#endif /* ILOGGER_HPP */
