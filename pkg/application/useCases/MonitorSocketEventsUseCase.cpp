#include "application/useCases/MonitorSocketEventsUseCase.hpp"

MonitorSocketEventsUseCase::MonitorSocketEventsUseCase() {
  try {
    this->_socketHandler = &SocketHandlerServiceLocator::get();
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Monitoring Socket: ") + e.what());
  }
}

MonitorSocketEventsUseCase::~MonitorSocketEventsUseCase() {}

MonitorSocketEventDTO MonitorSocketEventsUseCase::monitor() {
  InmemoryClientDatabase db = InmemoryClientDBServiceLocator::get();
  std::vector<pollfd> fds = db.listPollfds();
  nfds_t nfds = fds.size();
  try {
    this->_socketHandler->createPoll(fds.data(), nfds);
    for (size_t i = 0; i < nfds; ++i) {
      if (fds[i].revents & POLLIN) {
        if (fds[i].fd == this->_socketHandler->getServerSocket()) {
          return MonitorSocketEventDTO(MonitorSocketEventDTO::NewConnection, fds[i].fd);
        } else {
          return MonitorSocketEventDTO(MonitorSocketEventDTO::MessageReceived, fds[i].fd);
        }
      }
    }
    return MonitorSocketEventDTO(MonitorSocketEventDTO::Error, 0);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Monitoring Socket: ") + e.what());
  }
}
