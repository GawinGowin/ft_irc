#ifndef MONITORSOCKETEVENTSUSECASE_HPP
#define MONITORSOCKETEVENTSUSECASE_HPP

#include <poll.h>
#include <stdexcept>
#include <vector>

class MonitorSocketEventsUseCase {
public:
  MonitorSocketEventsUseCase();
  ~MonitorSocketEventsUseCase();
  void monitor();

private:
  MonitorSocketEventsUseCase(const MonitorSocketEventsUseCase &obj);
  MonitorSocketEventsUseCase &operator=(const MonitorSocketEventsUseCase &obj);
};

#endif /* MONITORSOCKETEVENTSUSECASE_HPP */
