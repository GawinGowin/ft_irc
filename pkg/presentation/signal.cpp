#include "signal.hpp"
#include "entrypoint.hpp"

static void sigHandler(int signum);

void setSignal() {
  std::signal(SIGINT, sigHandler);
  std::signal(SIGQUIT, sigHandler);
  std::signal(SIGPIPE, SIG_IGN);
}

static void sigHandler(int signum) { g_signal = signum; }
