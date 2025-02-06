#include "presentation/entrypoint.hpp"
#include "sentry.h"
#include <iostream>

#ifdef SENTRY_DSN
#else
#define SENTRY_DSN "https://examplePublicKey@o0.ingest.sentry.io/0"
#endif

volatile sig_atomic_t g_signal = 0;

int main(int argc, char **argv) {
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn(options, SENTRY_DSN);
  // This is also the default-path. For further information and recommendations:
  // https://docs.sentry.io/platforms/native/configuration/options/#database-path
  sentry_options_set_handler_path(options, "/usr/local/bin/crashpad_handler");
  sentry_options_set_database_path(options, ".sentry-native");
  sentry_options_set_release(options, "my-project-name@2.3.12");
  sentry_options_set_debug(options, 1);
  sentry_init(options);

  sentry_capture_event(sentry_value_new_message_event(
      /*   level */ SENTRY_LEVEL_INFO,
      /*  logger */ "custom",
      /* message */ "It works!"));

  try {
    entrypoint(argc, argv);
  } catch (const std::exception &e) {
    sentry_close();
    if (e.what() == std::string("Monitoring Socket: poll failed") && g_signal != 0) {
      return (0);
    }
    std::cerr << "Error: " << e.what() << std::endl;
    return (1);
  }
}
