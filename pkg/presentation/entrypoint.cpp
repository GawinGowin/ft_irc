#include "presentation/entrypoint.hpp"

void entrypoint(int argc, char **argv) {
  LogggerUseCase loggerWrapper(
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
        logger->debugss() << "Connection closed: " << clientFd;
        break;
      }
      status = RunCommandsUseCase::execute(msgDto);
      logger->debugss() << "Run command status: " << status;
      logger->debugss() << "Message received (fd/" << msgDto.getSenderId()
                        << "): " << msgDto.getMessage();
      break;
    case MonitorSocketEventDTO::Error:
      throw std::runtime_error("Failed to monitor socket events");
      break;
    }
  }
}
