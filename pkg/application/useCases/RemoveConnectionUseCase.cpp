#include "RemoveConnectionUseCase.hpp"

SendMsgDTO RemoveConnectionUseCase::remove(int clientFd) {
  MessageStreamVector messageStreams;

  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  InmemoryClientDatabase *db = &InmemoryClientDBServiceLocator::get();
  InmemoryChannelDatabase &channelDb = InmemoryChannelDBServiceLocator::get();
  MultiLogger *logger = LoggerServiceLocator::get();

  try {
    IClientAggregateRoot *client = db->getByFd(clientFd);
    if (client == NULL) {
      throw std::runtime_error("Client not found unexpectedly");
    }

    // 同じチャンネルにいるクライアントにメッセージを送信
    const IdToChannelMap &channels = channelDb.getDatabase();
    std::string quitMessage = ":Client closed connection";
    std::string clientPrefix = client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress();

    std::stringstream quitMsgss;
    quitMsgss << Message(clientPrefix, MessageConstants::CommandType::QUIT, quitMessage);

    std::map<std::string, Client *> quitMsgSentTargetsMap;

    for (IdToChannelMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {// 全部のチャンネルを取得
      IChannelAggregateRoot *channel = it->second;

      ChannelClientList &clientList = channel->getListConnects(); // 各チャンネルの接続者を確認
      if (clientList.isClientInList(client->getId())) { // クライアント(Quitした人)がチャンネルにいるか確認
        MessageStreamVector streams = MessageService::generateMessageToChannel(
          _socketHandler, client, db, channel, quitMsgss.str());
        messageStreams.insert(messageStreams.end(), streams.begin(), streams.end());
      }
    }

    ChannelService::removeClientFromAllChannels(client->getId());
    logger->debugss() << client->getId() << "(fd:" << clientFd << ") removed from all channels";

    // ユーザーが空ならば、チャンネルを削除する
    for (IdToChannelMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {
      IChannelAggregateRoot *channel = it->second;
      ChannelClientList &clientList = channel->getListConnects();
      
      if (clientList.getClients().empty()) {
        // チャンネルが空になったので削除
        logger->debugss() << "Channel " << channel->getName() << " is empty, removing it";
        channelDb.remove(channel->getName());
      } else if (channel->isOperator(client->getId())) {
        // Channel Operatorが抜けたときのオペレータの変更
        // 最初のクライアントを新しいオペレータに設定
        std::vector<std::string> &channelClients = clientList.getClients();
        if (!channelClients.empty()) {
          std::string newOperator = channelClients[0];
          channel->addOperator(newOperator);
          logger->debugss() << "New operator for channel " << channel->getName() 
                           << " is " << newOperator;

          // 新しいオペレータに通知
          IClientAggregateRoot *targetClient = db->getById(newOperator);
          if (targetClient != NULL) {
            MessageStream stream(_socketHandler, targetClient);
            // Get server name from configuration if available, or use "server" as fallback
            std::string serverName = "server";
            stream << Message(
                ":" + serverName, MessageConstants::ResponseCode::RPL_CHANNELMODEIS,
                targetClient->getNickName() + " " + channel->getName() + " +o " + newOperator);
            messageStreams.push_back(stream);
          }
        }
      }
    }

    db->remove(client->getId());
    _socketHandler->closeConnection(clientFd);
    logger->infoss() << "Connection closed: (fd:" << clientFd << ")";
    return SendMsgDTO(0, messageStreams);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("Remove connection: ") + e.what());
  }
}
