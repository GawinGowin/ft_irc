#include "RemoveConnectionUseCase.hpp"

void RemoveConnectionUseCase::remove(int clientFd) {
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();
  try {
    IClientAggregateRoot *client = db->getByFd(clientFd);
    if (client == NULL) {
      throw std::runtime_error("Client not found unexpectedly");
    }
    db->remove(client->getId());
    _socketHandler->closeConnection(clientFd);
    logger->infoss() << "Connection closed: (fd:" << clientFd << ")";
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Remove connection: ") + e.what());
  }
}
