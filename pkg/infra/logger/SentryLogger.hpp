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
    sentry_value_t crumb = sentry_value_new_breadcrumb("default", msg.c_str());
    sentry_value_set_by_key(crumb, "level", sentry_value_new_string("debug"));
    sentry_add_breadcrumb(crumb);
  };
  void info(std::string msg) {
    sentry_value_t crumb = sentry_value_new_breadcrumb("default", msg.c_str());
    sentry_value_set_by_key(crumb, "level", sentry_value_new_string("info"));
    sentry_add_breadcrumb(crumb);
  };
  void warning(std::string msg) {
    sentry_value_t crumb = sentry_value_new_breadcrumb("default", msg.c_str());
    sentry_value_set_by_key(crumb, "level", sentry_value_new_string("warning"));
    sentry_add_breadcrumb(crumb);
  };
  void error(std::string msg) {
    sentry_value_t crumb = sentry_value_new_breadcrumb("default", msg.c_str());
    sentry_value_set_by_key(crumb, "level", sentry_value_new_string("error"));
    sentry_add_breadcrumb(crumb);
  };
  void fatal(std::string msg) {
    sentry_capture_event(sentry_value_new_message_event(
        /*   level */ SENTRY_LEVEL_FATAL,
        /*  logger */ "custom",
        /* message */ msg.c_str()));
  }
};

#endif

#endif /* SENTRYLOGGER_HPP */
