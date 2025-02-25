#include "presentation/entrypoint.hpp"
#include <iostream>

volatile sig_atomic_t g_signal = 0;

#if defined(USE_SENTRY)
#include <sentry.h>

int main(int argc, char **argv) {
  char *sentryDsn = getenv("SENTRY_DSN");
  if (sentryDsn == NULL) {
    std::cerr << "Error: SENTRY_DSN environment variable is not set" << std::endl;
    return (1);
  }
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn(options, sentryDsn);
  // This is also the default-path. For further information and recommendations:
  // https://docs.sentry.io/platforms/native/configuration/options/#database-path
  sentry_options_set_handler_path(options, "/usr/local/bin/crashpad_handler");
  sentry_options_set_database_path(options, ".sentry-native");
  sentry_options_set_release(options, "ft_irc@0.1");
  sentry_options_set_debug(options, 1);
  sentry_options_set_sample_rate(options, 1);
  sentry_init(options);
  try {
    entrypoint(argc, argv);
  } catch (const std::exception &e) {
    int statusCode = 1;
    if (e.what() == std::string("Monitoring Socket: poll failed") && g_signal != 0) {
      statusCode = 0;
    } else {
      std::cerr << "Error: " << e.what() << std::endl;
    }
    sentry_close();
    return (statusCode);
  }
}
#else

int main(int argc, char **argv) {
  try {
    entrypoint(argc, argv);
  } catch (const std::exception &e) {
    if (e.what() == std::string("Monitoring Socket: poll failed") && g_signal != 0) {
      return (0);
    }
    std::cerr << "Error: " << e.what() << std::endl;
    return (1);
  }
}

#endif
