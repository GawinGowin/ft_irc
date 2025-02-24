#include "RemoveConnectionUseCase.hpp"

void RemoveConnectionUseCase::remove(int clientFd) {
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  try {
    MultiLogger *logger = LoggerServiceLocator::get();
    _socketHandler->closeConnection(clientFd);
    db->removeFdsByFd(clientFd);
    // TODO: チャンネルからもユーザーを削除する処理を追加
    logger->infoss() << "Connection closed: (fd:" << clientFd << ")";
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Remove connection: ") + e.what());
  }
}