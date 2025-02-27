#include "signal.hpp"
#include "entrypoint.hpp"

static void sigHandler(int signum);

void setSignal() {
  std::signal(SIGINT, sigHandler);
  std::signal(SIGQUIT, sigHandler);
}

static void sigHandler(int signum) { g_signal = signum; }
