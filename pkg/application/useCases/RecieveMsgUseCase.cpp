#include "application/useCases/RecieveMsgUseCase.hpp"

RecievedMsgDTO RecieveMsgUseCase::recieve(const MonitorSocketEventDTO &event) {
  SocketHandler *_socketHandler;

  std::string msg;
  try {
    _socketHandler = &SocketHandlerServiceLocator::get();
    msg = _socketHandler->receiveMsg(event.getConnectionFd());
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("RecieveMsgUseCase: ") + e.what());
  }
  return (RecievedMsgDTO(msg, event.getConnectionFd()));
}
