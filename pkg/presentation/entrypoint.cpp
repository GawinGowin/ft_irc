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
  std::cout << "Start Listening..." << std::endl;
  while (true) {
    eventDto = monitorSocketEventsUseCase.monitor();
    // handle dto
    RecievedMsgDTO msgDto;
    switch (eventDto.getEvent()) {
    case MonitorSocketEventDTO::NewConnection:
      AcceptConnectionUseCase::accept();
      std::cout << "New connection" << std::endl;
      break;
    case MonitorSocketEventDTO::MessageReceived:
      msgDto = RecieveMsgUseCase::recieve(eventDto);
      std::cout << "Message received from client #" << msgDto.getSenderId() << ": "
                << msgDto.getMessage() << std::endl;
      break;
    default:
      throw std::runtime_error("Failed to monitor socket events");
    }
  }
}
