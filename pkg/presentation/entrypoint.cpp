#include "presentation/entrypoint.hpp"

void entrypoint(int argc, char **argv) {
  LogggerWrapper loggerWrapper(
      LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE, "ft_irc.log");
  MultiLogger *logger = LoggerServiceLocator::get();
  StartServerDTO dto(argc, argv);
  StartServerUseCase startServerUseCase(dto);
  setSignal();
  try {
    startServerUseCase.execute();
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Failed to start server: ") + e.what());
  }
  MonitorSocketEventsUseCase monitorSocketEventsUseCase;
  MonitorSocketEventDTO eventDto;
  RecievedMsgDTO msgDto;
  int status;
  logger->info("Start Listening...");
  while (true) {
    eventDto = monitorSocketEventsUseCase.monitor();
    // handle dto
    switch (eventDto.getEvent()) {
    case MonitorSocketEventDTO::NewConnection:
      AcceptConnectionUseCase::accept();
      logger->debug("New connection");
      break;
    case MonitorSocketEventDTO::MessageReceived:
      msgDto = RecieveMsgUseCase::recieve(eventDto);
      if (msgDto.getMessage().size() == 0) {
        int clientFd = eventDto.getConnectionFd();
        RemoveConnectionUseCase::remove(clientFd);
        std::cout << "Connection closed: " << clientFd << std::endl;
        break;
      }
      status = RunCommandsUseCase::execute(msgDto);
      std::cout << "Run command status: " << status << std::endl;
      std::cout << "Message received (fd/" << msgDto.getSenderId() << "): " << msgDto.getMessage()
                << std::endl;
      break;
    case MonitorSocketEventDTO::Error:
      throw std::runtime_error("Failed to monitor socket events");
      break;
    }
  }
}
