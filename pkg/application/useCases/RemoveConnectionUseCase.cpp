#include "RemoveConnectionUseCase.hpp"

SendMsgDTO RemoveConnectionUseCase::remove(int clientFd) {
  MessageStreamVector messageStreams;

  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();

  try {
    IClientAggregateRoot *client = db->getByFd(clientFd);
    if (client == NULL) {
      throw std::runtime_error("Client not found unexpectedly");
    }
    // TODO: 同じチャンネルにいるクライアントにメッセージを送信
    // 例: `:A!~A@172.18.0.1 QUIT :Client closed connection`
    ChannelService::removeClientFromAllChannels(client->getId());
    logger->debugss() << client->getId() << "(fd:" << clientFd << ") removed from all channels";

    // TODO: ユーザーが空ならば、チャンネルを削除する機構を追加する
    // TODO: Channel Operatorが抜けたときのオペレータの変更

    db->remove(client->getId());
    _socketHandler->closeConnection(clientFd);
    logger->infoss() << "Connection closed: (fd:" << clientFd << ")";
    return SendMsgDTO(0, messageStreams);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Remove connection: ") + e.what());
  }
}
