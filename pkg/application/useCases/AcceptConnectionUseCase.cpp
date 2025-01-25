#include "application/useCases/AcceptConnectionUseCase.hpp"

void AcceptConnectionUseCase::accept() {
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  try {
    int clientSocket = _socketHandler->acceptConnection();
    pollfd pollfd = {clientSocket, POLLIN, 0};
    Client client = Client(clientSocket, pollfd);
    db->add(client);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Accept connection: ") + e.what());
  }
}
