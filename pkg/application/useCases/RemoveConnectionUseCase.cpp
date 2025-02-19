#include "RemoveConnectionUseCase.hpp"

void RemoveConnectionUseCase::remove(int clientFd) {
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  try {
    _socketHandler->closeConnection(clientFd);
    db->removeFdsByFd(clientFd);
    // TODO: チャンネルからもユーザーを削除する処理を追加
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Remove connection: ") + e.what());
  }
}