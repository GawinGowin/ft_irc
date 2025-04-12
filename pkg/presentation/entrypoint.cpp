#include "presentation/entrypoint.hpp"

static const int logTypes = LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE;
volatile sig_atomic_t g_signal = 0;

void entrypoint(int argc, char **argv) {
  LogggerUseCase loggerWrapper(logTypes, "ft_irc.log");
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

  std::vector<RecievedMsgDTO> recievedMsgDtos;
  SendMsgDTO sendMsgDto;

  logger->infoss() << "Start Listening port:"
                   << ConfigsServiceLocator::get().getConfigs().Global.Port;
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
      recievedMsgDtos = RecieveMsgUseCase::recieve(eventDto);
      for (std::vector<RecievedMsgDTO>::iterator it = recievedMsgDtos.begin();
           it != recievedMsgDtos.end(); ++it) {
        RecievedMsgDTO &recievedMsgDto = *it;
        sendMsgDto = RunCommandsUseCase::execute(recievedMsgDto);
        SendMsgFromServerUseCase::send(sendMsgDto);
        if (recievedMsgDto.getClient()->getClientType() & CLIENT_DISCONNECT) {
          RemoveConnectionUseCase::remove(eventDto.getConnectionFd());
        }
      }
      break;
    case MonitorSocketEventDTO::Error:
      logger->fatal("Failed to monitor socket events");
      break;
    }
  }
  logger->infoss() << "Interrupt signal (" << g_signal << ") received";
  logger->info("Stop server");
}
