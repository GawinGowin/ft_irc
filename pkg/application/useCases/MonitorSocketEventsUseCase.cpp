#include "MonitorSocketEventsUseCase.hpp"

MonitorSocketEventsUseCase::MonitorSocketEventsUseCase(IServerRepository *serverRepository)
    : _serverRepository(serverRepository) {}

MonitorSocketEventDTO MonitorSocketEventsUseCase::monitor(pollfd *fds, nfds_t nfds) {
  const int w_addr = this->_serverRepository->getWatchingAddress();
  int activity = poll(fds, nfds, -1);
  if (activity < 0) {
    return MonitorSocketEventDTO(MonitorSocketEventDTO::Error, 0);
  }
  for (size_t i = 0; i < nfds; ++i) {
    if (fds[i].revents & POLLIN) {
      if (fds[i].fd == w_addr) {
        return MonitorSocketEventDTO(MonitorSocketEventDTO::NewConnection, fds[i].fd);
      } else {
        return MonitorSocketEventDTO(MonitorSocketEventDTO::MessageReceived, fds[i].fd);
      }
    }
  }
}
