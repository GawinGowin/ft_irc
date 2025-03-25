#include "presentation/entrypoint.hpp"

#if defined(USE_SENTRY)
const int logTypes =
    LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE | LoggerServiceLocator::SENTRY;
#else
const int logTypes = LoggerServiceLocator::CONSOLE | LoggerServiceLocator::FILE;
#endif

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
  RecievedMsgDTO recievedMsgDto;
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
      recievedMsgDto = RecieveMsgUseCase::recieve(eventDto);
      if (recievedMsgDto.getMessage().size() == 0) {
        int clientFd = eventDto.getConnectionFd();
        RemoveConnectionUseCase::remove(clientFd);
        break;
      }
      sendMsgDto = RunCommandsUseCase::execute(recievedMsgDto);
      SendMsgFromServerUseCase::send(sendMsgDto);
      if (recievedMsgDto.getClient()->getClientType() & CLIENT_DISCONNECT) {
        RemoveConnectionUseCase::remove(eventDto.getConnectionFd());
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
