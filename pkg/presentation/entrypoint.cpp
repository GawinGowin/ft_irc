#include "presentation/entrypoint.hpp"

void entrypoint(int argc, char **argv) {
  StartServerDTO dto(argc, argv);
  StartServerUseCase startServerUseCase(dto);
  try {
    startServerUseCase.execute();
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Failed to start server: ") + e.what());
  }
  MonitorSocketEventsUseCase monitorSocketEventsUseCase;
  MonitorSocketEventDTO eventDto;
  while (true) {
    eventDto = monitorSocketEventsUseCase.monitor();
    // handle dto
    switch (eventDto.getEvent()) {
    case MonitorSocketEventDTO::NewConnection:
      // handle client connected
      break;
    case MonitorSocketEventDTO::MessageReceived:
      // handle client disconnected
      break;
    default:
      throw std::runtime_error("Failed to monitor socket events");
    }
  }
}
