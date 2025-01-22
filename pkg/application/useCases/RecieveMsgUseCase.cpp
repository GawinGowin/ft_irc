#include "application/useCases/RecieveMsgUseCase.hpp"

std::string RecieveMsgUseCase::recieve(const MonitorSocketEventDTO &event) {
  std::string msg;
  try {
    SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
    msg = _socketHandler->receiveMsg(event.getConnectionFd());
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("RecieveMsgUseCase: ") + e.what());
  }
  return msg;
}
