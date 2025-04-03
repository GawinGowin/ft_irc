#include "RemoveConnectionUseCase.hpp"

void RemoveConnectionUseCase::remove(int clientFd) {
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();
  db->removeFdsByFd(clientFd);
  try {
    _socketHandler->closeConnection(clientFd);
    logger->infoss() << "Connection closed: (fd:" << clientFd << ")";
  } catch (const std::runtime_error &e) {
    logger->errorss() << "Failed to remove connection: " << e.what() << " (fd:" << clientFd << ")";
  }
}
