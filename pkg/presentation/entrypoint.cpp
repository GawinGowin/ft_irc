#include "presentation/entrypoint.hpp"

static std::pair<int, std::string> validateArgs(int argc, char **argv);

void entrypoint(int argc, char **argv) {
  StartServerDTO dto(validateArgs(argc, argv));
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
    std::string msg;
    switch (eventDto.getEvent()) {
    case MonitorSocketEventDTO::NewConnection:
      AcceptConnectionUseCase::accept();
      std::cout << "New connection" << std::endl;
      break;
    case MonitorSocketEventDTO::MessageReceived:
      msg = RecieveMsgUseCase::recieve(eventDto);
      std::cout << "Message received: " << msg << std::endl;
      break;
    default:
      throw std::runtime_error("Failed to monitor socket events");
    }
  }
}

static std::pair<int, std::string> validateArgs(int argc, char **argv) {
  if (argc != 3) {
    throw std::runtime_error("Invalid number of arguments");
  }
  for (int i = 0; argv[1][i]; i++) {
    if (!std::isdigit(argv[1][i]) || argv[1][0] == '0') {
      throw std::runtime_error("Invalid port");
    }
  }
  std::stringstream ss(argv[1]);
  unsigned int port;
  ss >> port;
  if (ss.fail() || port > 65535) {
    throw std::runtime_error("Invalid port");
  }
  std::string password(argv[2]);
  if (password.empty() || (password.find(' ') != std::string::npos)) {
    throw std::runtime_error("Invalid password");
  }
  std::cout << "Port: " << port << " Password: " << password << std::endl;
  return std::make_pair(port, password);
}
