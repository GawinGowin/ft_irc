#include "application/useCases/StartServerUseCase.hpp"

const int maxConnections = 10;
const int maxBufferSize = 1024;

StartServerUseCase::StartServerUseCase(const StartServerDTO &dto) {
  try {
    SocketHandlerServiceLocator::init(dto.getAddress(), dto.getPort(), maxConnections, maxBufferSize);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("init SocketHandler: ") + e.what());
  }
}

StartServerUseCase::~StartServerUseCase() {
  SocketHandlerServiceLocator::cleanup();
}

void StartServerUseCase::execute() {
  try {
    SocketHandlerServiceLocator::get().initializeSocket();
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("init socket: ") + e.what());
  }
  InmemoryClientDatabase db = InmemoryClientDBServiceLocator::get();
  pollfd serverPollfd = SocketHandlerServiceLocator::get().getServerPollfd();
  Client client(0, serverPollfd);
  db.add(client);
}
