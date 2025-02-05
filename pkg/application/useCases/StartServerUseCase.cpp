#include "application/useCases/StartServerUseCase.hpp"

StartServerUseCase::StartServerUseCase(const StartServerDTO &dto) {
  ConfigsLoader &loader = ConfigsServiceLocator::get();
  loader.setPort(dto.getPort());
  loader.setPassword(dto.getPassword());
  const Configs &conf = loader.getConfigs();
  try {
    SocketHandlerServiceLocator::init(
        conf.Global.Listen, conf.Global.Port, conf.Limits.MaxConnections,
        conf.Limits.MaxBufferSize);
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
