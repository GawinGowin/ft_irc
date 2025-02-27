#ifndef STREAMLOGGER_HPP
#define STREAMLOGGER_HPP

#include "infra/logger/ILogger.hpp"
#include "infra/logger/LogLevel.hpp"
#include "infra/logger/LogStream.hpp"

/**
 * @class StreamLogger
 * @brief ログ出力をストリーム形式で行うためのクラス
 *
 * このクラスは、ILogger インターフェースを使用してログメッセージを
 * ストリーム形式で出力するための機能を提供します。
 */
class StreamLogger {
public:
  /**
   * @brief コンストラクタ
   * @param logger ILogger インターフェースを実装したロガーオブジェクト
   */
  StreamLogger(ILogger *logger) : _logger(logger) {}
  ~StreamLogger() {}
  /**
   * @brief TRACE レベルのログストリームを取得
   * @return TRACE レベルの LogStream オブジェクト
   */
  LogStream tracess(void) { return LogStream(_logger, TRACE); }
  /**
   * @brief DEBUG レベルのログストリームを取得
   * @return DEBUG レベルの LogStream オブジェクト
   */
  LogStream debugss(void) { return LogStream(_logger, DEBUG); }
  /**
   * @brief INFO レベルのログストリームを取得
   * @return INFO レベルの LogStream オブジェクト
   */
  LogStream infoss(void) { return LogStream(_logger, INFO); }
  /**
   * @brief WARNING レベルのログストリームを取得
   * @return WARNING レベルの LogStream オブジェクト
   */
  LogStream warningss(void) { return LogStream(_logger, WARNING); }
  /**
   * @brief ERROR レベルのログストリームを取得
   * @return ERROR レベルの LogStream オブジェクト
   */
  LogStream errorss(void) { return LogStream(_logger, ERROR); }

private:
  ILogger *_logger;
};

#endif /* STREAMLOGGER_HPP */
