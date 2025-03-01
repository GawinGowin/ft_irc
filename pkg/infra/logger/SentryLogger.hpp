#ifndef SENTRYLOGGER_HPP
#define SENTRYLOGGER_HPP

#if defined(USE_SENTRY)

#include "infra/logger/ILogger.hpp"
#include "infra/logger/StreamLogger.hpp"

#include <sentry.h>

class SentryLogger : virtual public ILogger, virtual public StreamLogger {
public:
  SentryLogger() : StreamLogger(this) {}
  void trace(std::string msg) {
    (void)msg; // do nothing
  };
  void debug(std::string msg) {
    sentry_capture_event(sentry_value_new_message_event(
        /*   level */ SENTRY_LEVEL_DEBUG,
        /*  logger */ "custom",
        /* message */ msg.c_str()));
  };
  void info(std::string msg) {
    sentry_capture_event(sentry_value_new_message_event(
        /*   level */ SENTRY_LEVEL_INFO,
        /*  logger */ "custom",
        /* message */ msg.c_str()));
  };
  void warning(std::string msg) {
    sentry_capture_event(sentry_value_new_message_event(
        /*   level */ SENTRY_LEVEL_WARNING,
        /*  logger */ "custom",
        /* message */ msg.c_str()));
  };
  void error(std::string msg) {
    sentry_capture_event(sentry_value_new_message_event(
        /*   level */ SENTRY_LEVEL_ERROR,
        /*  logger */ "custom",
        /* message */ msg.c_str()));
  };
};

#endif

#endif /* SENTRYLOGGER_HPP */
