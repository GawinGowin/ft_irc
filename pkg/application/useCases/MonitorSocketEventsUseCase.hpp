#ifndef MONITORSOCKETEVENTSUSECASE_HPP
#define MONITORSOCKETEVENTSUSECASE_HPP

#include "IServerRepository.hpp"
#include "IServerServiceRepository.hpp"
#include "MonitorSocketEventDTO.hpp"

#include <poll.h>
#include <stdexcept>

class MonitorSocketEventsUseCase {
public:
  MonitorSocketEventsUseCase(IServerRepository *serverRepository);

  MonitorSocketEventDTO monitor();

private:
  MonitorSocketEventsUseCase();
  MonitorSocketEventsUseCase(const MonitorSocketEventsUseCase &obj);
  MonitorSocketEventsUseCase &operator=(const MonitorSocketEventsUseCase &obj);

  IServerRepository *_serverRepository;
};

#endif /* MONITORSOCKETEVENTSUSECASE_HPP */
