#ifndef MONITORSOCKETEVENTSUSECASE_HPP
#define MONITORSOCKETEVENTSUSECASE_HPP

#include <poll.h>
#include <stdexcept>
#include <vector>

#include "application/dto/MonitorSocketEventDTO.hpp"
#include "infra/database/InmemoryClientDBServiceLocator.hpp"
#include "infra/database/InmemoryClientDatabase.hpp"
#include "infra/socket/SocketHandlerServiceLocator.hpp"

class MonitorSocketEventsUseCase {
public:
  MonitorSocketEventsUseCase();
  ~MonitorSocketEventsUseCase();
  MonitorSocketEventDTO monitor();

private:
  MonitorSocketEventsUseCase(const MonitorSocketEventsUseCase &obj);
  MonitorSocketEventsUseCase &operator=(const MonitorSocketEventsUseCase &obj);

  ISocketHandler *_socketHandler;
};

#endif /* MONITORSOCKETEVENTSUSECASE_HPP */
