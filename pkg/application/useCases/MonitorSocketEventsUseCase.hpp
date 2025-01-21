#ifndef MONITORSOCKETEVENTSUSECASE_HPP
#define MONITORSOCKETEVENTSUSECASE_HPP

#include <poll.h>
#include <stdexcept>
#include <vector>

class MonitorSocketEventsUseCase {
public:
  MonitorSocketEventsUseCase();

  void monitor();

private:
  MonitorSocketEventsUseCase();
  MonitorSocketEventsUseCase(const MonitorSocketEventsUseCase &obj);
  MonitorSocketEventsUseCase &operator=(const MonitorSocketEventsUseCase &obj);
};

#endif /* MONITORSOCKETEVENTSUSECASE_HPP */
