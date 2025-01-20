#ifndef MONITORSOCKETEVENTSUSECASE_HPP
#define MONITORSOCKETEVENTSUSECASE_HPP

#include "IClientRepository.hpp"
#include "IServerRepository.hpp"
#include "MonitorSocketEventDTO.hpp"
#include <poll.h>
#include <stdexcept>
#include <vector>

class MonitorSocketEventsUseCase {
public:
  MonitorSocketEventsUseCase(IServerRepository *serverRepository);

  MonitorSocketEventDTO monitor(pollfd *fds, nfds_t nfds);

private:
  MonitorSocketEventsUseCase();
  MonitorSocketEventsUseCase(const MonitorSocketEventsUseCase &obj);
  MonitorSocketEventsUseCase &operator=(const MonitorSocketEventsUseCase &obj);

  IServerRepository *_serverRepository;
};

#endif /* MONITORSOCKETEVENTSUSECASE_HPP */
