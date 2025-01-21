#include "presentation/entrypoint.hpp"

void entrypoint(int argc, char **argv) {
  // TODO: 以下のコードは仮実装。適当なusecase、もしくは関数でいい感じにする
  (void)argc;
  (void)argv;
  const std::string addr = "127.0.0.1";
  const int port = 8080;
  const std::string password = "password";

  IServerRepository *server = ServerFactory::createServer();
  IServerService *serverService = ServerFactory::createServerService();
  StartServerUseCase startServerUseCase(server, serverService);
  try {
    startServerUseCase.execute(addr, port, password);
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
  delete server;
  delete serverService;
}
