#ifndef LOGGERSERVICELOCATOR_HPP
#define LOGGERSERVICELOCATOR_HPP

#include "infra/logger/ConsoleLogger.hpp"
#include "infra/logger/FileLogger.hpp"
#include "infra/logger/ILogger.hpp"
#include "infra/logger/MultiLogger.hpp"
#include <stdexcept>
#include <string>

/**
 * @class LoggerServiceLocator
 * @brief ロガーサービスのロケータクラス。
 *
 * このクラスは、異なるタイプのロガー（コンソール、ファイル、Sentry）を管理し、
 * 必要に応じて初期化、取得、クリーンアップを行います。
 */
class LoggerServiceLocator {
public:
  /**
   * @enum LoggerServiceLocator::loggerType
   * @brief ロガーのタイプを示す列挙型。
   *
   * - CONSOLE: コンソールロガー
   * - FILE: ファイルロガー
   * - SENTRY: Sentryロガー
   */
  enum loggerType { CONSOLE = 1 << 0, FILE = 1 << 1, SENTRY = 1 << 2 };

  /**
   * @fn static void LoggerServiceLocator::init(int type, std::string logFile = "")
   * @brief ロガーを初期化します。
   *
   * @param type 初期化するロガーのタイプ（ビットマスク）。
   * @param logFile ファイルロガーを使用する場合のログファイルのパス。
   * @throws std::runtime_error ロガーが既に初期化されている場合、またはファイルロガーのログファイルが空の場合。
   */
  static void init(int type, std::string logFile = "") {
    if (_logger != NULL) {
      throw std::runtime_error("LoggerServiceLocator is already initialized");
    }
    _logger = new MultiLogger();
    if (type & CONSOLE) {
      _logger->addLogger(new ConsoleLogger());
    }
    if (type & FILE) {
      if (logFile.empty()) {
        throw std::runtime_error("logFile is empty");
      }
      _logger->addLogger(new FileLogger(logFile));
    }
    _logger->trace("Logger initialized");
  }

  /**
   * @fn static MultiLogger* LoggerServiceLocator::get()
   * @brief 初期化されたロガーを取得します。
   *
   * @return 初期化されたMultiLoggerのポインタ。
   * @throws std::runtime_error ロガーが初期化されていない場合。
   */
  static MultiLogger *get() {
    if (_logger == NULL) {
      throw std::runtime_error("LoggerServiceLocator is not initialized");
    }
    return _logger;
  }

  /**
   * @fn static void LoggerServiceLocator::clean()
   * @brief ロガーをクリーンアップします。
   *
   * ロガーのクリーンアップ処理を行い、メモリを解放します。
   */
  static void clean() {
    _logger->trace("Logger cleaned");
    if (_logger != NULL) {
      delete _logger;
      _logger = NULL;
    }
  }

private:
  static MultiLogger *_logger;
};

#endif /* LOGGERSERVICELOCATOR_HPP */
