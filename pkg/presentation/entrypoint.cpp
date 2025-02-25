#include "presentation/entrypoint.hpp"

const int logTypes =
    LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE | LoggerServiceLocator::SENTRY;

void entrypoint(int argc, char **argv) {
  LogggerUseCase loggerWrapper(logTypes, "ft_irc.log");
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
  RecievedMsgDTO recievedMsgDto;
  SendMsgDTO sendMsgDto;

  logger->info("Start Listening...");
  while (true) {
    eventDto = monitorSocketEventsUseCase.monitor();
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
      throw std::runtime_error("Failed to monitor socket events");
      break;
    }
  }
}
