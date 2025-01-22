#include "presentation/entrypoint.hpp"

const std::string password = "password";

void entrypoint(int argc, char **argv) {
  StartServerDTO dto(argc, argv);
  StartServerUseCase startServerUseCase(dto);
  try {
    startServerUseCase.execute();
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Failed to start server: ") + e.what());
  }
  MonitorSocketEventsUseCase monitorSocketEventsUseCase(server);
  MonitorSocketEventDTO dto;
  while (true) {
    dto = monitorSocketEventsUseCase.monitor();
    // handle dto
    switch (dto.getEvent()) {
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
