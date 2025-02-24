#include "application/useCases/RecieveMsgUseCase.hpp"

RecievedMsgDTO RecieveMsgUseCase::recieve(const MonitorSocketEventDTO &event) {
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  SocketHandler *socketHandler = &SocketHandlerServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();

  std::string msg;
  int connectionFd;

  connectionFd = event.getConnectionFd();
  IClientAggregateRoot *client = db->getByFd(connectionFd);
  try {
    msg = socketHandler->receiveMsg(connectionFd);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("RecieveMsgUseCase: ") + e.what());
  }
  logger->tracess() << "Message recieved from client: " << client->getAddress()
                    << " (fd: " << client->getSocketFd() << ")";
  return (RecievedMsgDTO(msg, client));
}
