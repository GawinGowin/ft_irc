#include "application/useCases/AcceptConnectionUseCase.hpp"

#include <iostream>

void AcceptConnectionUseCase::accept() {
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  struct sockaddr_in clientAddr;
  try {
    int clientSocket = _socketHandler->acceptConnection(&clientAddr);
    std::string clientIp = _socketHandler->getClientIp(clientAddr);

    ConnectionInfo conn(clientSocket, clientIp);

    pollfd pollfd = {clientSocket, POLLIN, 0};
    Client client = Client(clientSocket, pollfd);
    db->add(client);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Accept connection: ") + e.what());
  }
}
