#include "application/useCases/AcceptConnectionUseCase.hpp"

#include <iostream>

void AcceptConnectionUseCase::accept() {
  MultiLogger *logger = LoggerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  struct sockaddr_in clientAddr;
  try {
    int clientSocket = _socketHandler->acceptConnection(&clientAddr);
    std::string clientIp = _socketHandler->getClientIp(clientAddr);

    pollfd pollfd = {clientSocket, POLLIN, 0};
    Client client = Client(clientIp, pollfd);
    db->add(client);
    logger->infoss() << "New connection accepted: " << clientIp << " (fd: " << clientSocket << ")";
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Accept connection: ") + e.what());
  }
}
