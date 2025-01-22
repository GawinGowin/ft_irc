#include "application/useCases/StartServerUseCase.hpp"

const int maxConnections = 10;
const int maxBufferSize = 1024;

StartServerUseCase::StartServerUseCase(const StartServerDTO &dto) {
  try {
    ServerPassword *password = new ServerPassword(dto.getPassword());
    SocketHandlerServiceLocator::init(
        dto.getAddress(), dto.getPort(), maxConnections, maxBufferSize,
        reinterpret_cast<IServerPasswordAggregateRoot *>(password));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("init SocketHandler: ") + e.what());
  }
}

StartServerUseCase::~StartServerUseCase() { SocketHandlerServiceLocator::cleanup(); }

void StartServerUseCase::execute() {
  try {
    SocketHandlerServiceLocator::get().initializeSocket();
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("init socket: ") + e.what());
  }
  pollfd serverPollfd = SocketHandlerServiceLocator::get().getServerPollfd();
  Client client(serverPollfd.fd, serverPollfd);
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  db->add(client);
}
