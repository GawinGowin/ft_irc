#include "application/useCases/RecieveMsgUseCase.hpp"

static const std::string CRLF = "\r\n";

std::vector<RecievedMsgDTO> RecieveMsgUseCase::recieve(const MonitorSocketEventDTO &event) {
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  SocketHandler *socketHandler = &SocketHandlerServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();

  std::string msg;
  int connectionFd;
  std::vector<RecievedMsgDTO> dtos;

  connectionFd = event.getConnectionFd();
  IClientAggregateRoot *client = db->getByFd(connectionFd);

  try {
    msg = socketHandler->receiveMsg(connectionFd);
    logger->tracess() << "Message from client: " << client->getAddress()
                      << " (fd: " << client->getSocketFd() << ") " << msg;
    if (msg.empty()) {
      dtos.push_back(RecievedMsgDTO(RecievedMsgDTO(msg, client)));
      return dtos;
    }
    size_t start = 0;
    size_t end = 0;
    while ((end = msg.find(CRLF, start)) != std::string::npos) {
      std::string command = msg.substr(start, end - start + CRLF.length());
      dtos.push_back(RecievedMsgDTO(RecievedMsgDTO(command, client)));
      start = end + CRLF.length();
    }
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("RecieveMsgUseCase: ") + e.what());
  }
  return dtos;
}
