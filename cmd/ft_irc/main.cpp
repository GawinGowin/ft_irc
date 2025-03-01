#include "presentation/entrypoint.hpp"
#include <iostream>

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
    sentry_close();
    std::cerr << "Error: " << e.what() << std::endl;
    return (1);
  }
}
#else

int main(int argc, char **argv) {
  try {
    entrypoint(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return (1);
  }
}

#endif
