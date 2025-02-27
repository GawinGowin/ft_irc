#include "presentation/entrypoint.hpp"

void entrypoint(int argc, char **argv) {
  LogggerUseCase loggerWrapper(
      LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE, "ft_irc.log");
  MultiLogger *logger = LoggerServiceLocator::get();

  StartServerDTO dto(argc, argv);
  if (dto.isError()) {
    logger->fatal(dto.getErrorMessage());
  }
  StartServerUseCase startServerUseCase(dto);
  setSignal();
  try {
    startServerUseCase.execute();
  } catch (const std::runtime_error &e) {
    logger->fatal(std::string("Failed to start server: ") + e.what());
  }
  MonitorSocketEventsUseCase monitorSocketEventsUseCase;
  MonitorSocketEventDTO eventDto;
  RecievedMsgDTO recievedMsgDto;
  SendMsgDTO sendMsgDto;

  logger->info("Start Listening...");
  while (!g_signal) {
    try {
      eventDto = monitorSocketEventsUseCase.monitor();
    } catch (const std::runtime_error &e) {
      if (g_signal) {
        break;
      }
      logger->fatal(e.what());
    }
    switch (eventDto.getEvent()) {
    case MonitorSocketEventDTO::NewConnection:
      AcceptConnectionUseCase::accept();
      break;
    case MonitorSocketEventDTO::MessageReceived:
      recievedMsgDto = RecieveMsgUseCase::recieve(eventDto);
      if (recievedMsgDto.getMessage().size() == 0) {
        int clientFd = eventDto.getConnectionFd();
        RemoveConnectionUseCase::remove(clientFd);
        break;
      }
      sendMsgDto = RunCommandsUseCase::execute(recievedMsgDto);
      SendMsgFromServerUseCase::send(recievedMsgDto.getClient(), sendMsgDto);
      break;
    case MonitorSocketEventDTO::Error:
      logger->fatal("Failed to monitor socket events");
      break;
    }
  }
  logger->infoss() << "Interrupt signal (" << g_signal << ") received";
  logger->info("Stop server...");
}
