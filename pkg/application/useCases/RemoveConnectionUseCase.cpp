#include "RemoveConnectionUseCase.hpp"

void RemoveConnectionUseCase::remove(int clientFd) {
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();
  try {
    db->removeFdsByFd(clientFd);
    _socketHandler->closeConnection(clientFd);
    logger->infoss() << "Connection closed: (fd:" << clientFd << ")";
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Remove connection: ") + e.what());
  }
}
