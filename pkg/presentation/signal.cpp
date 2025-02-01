#include "signal.hpp"

static void sigHandler(int signum);

void setSignal() {
  std::signal(SIGINT, sigHandler);
  std::signal(SIGQUIT, sigHandler);
}

static void sigHandler(int signum) {
  std::cout << "\nInterrupt signal (" << signum << ") received.\nStop server..." << std::endl;
  exit(0);
}
