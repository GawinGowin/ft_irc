#include "application/commands/Quit.hpp"

Quit::Quit(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_channelDB = &InmemoryChannelDBServiceLocator::get();
  this->_clientDB = &InmemoryClientDBServiceLocator::get();
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
  this->_socketHandler = &SocketHandlerServiceLocator::get();
}

SendMsgDTO Quit::execute() {
  // IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();

  MessageStreamVector messageStreams;

  // 離脱するClientと同じチャンネルに所属する人にQuitメッセージを送信する
  const std::string quitMessage = ":Client closed connection";
  const std::string prefix =
      client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress();
  std::stringstream quitMsgss;
  quitMsgss << Message(prefix, MessageConstants::QUIT, quitMessage);
  const IdToChannelMap &channels = this->_channelDB->getDatabase();
  std::map<std::string, IClientAggregateRoot *> clientQuitTargetsMap;
  for (IdToChannelMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {
    IChannelAggregateRoot *channel = it->second;

    ChannelClientList &clientList = channel->getListConnects();
    if (clientList.isClientInList(client->getId())) {

      const std::vector<std::string> &members = clientList.getClients();
      for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end();
           ++it) {
        IClientAggregateRoot *member = this->_clientDB->getById(*it);
        if (member && member->getId() != client->getId()) {
          clientQuitTargetsMap[*it] = member;
        }
      }
    }
  }
  if (!clientQuitTargetsMap.empty()) {
    for (std::map<std::string, IClientAggregateRoot *>::const_iterator it =
             clientQuitTargetsMap.begin();
         it != clientQuitTargetsMap.end(); ++it) {
      MessageStream stream(this->_socketHandler, it->second);
      stream << quitMsgss.str();
      messageStreams.push_back(stream);
    }
  }

  // チャンネルからクライアントを削除
  ChannelService::removeClientFromAllChannels(client->getId());

  this->_logger->debugss() << client->getId() << "(fd:" << client->getSocketFd()
                           << ") removed from all channels";

  // ユーザーが空ならば、チャンネルを削除する
  for (IdToChannelMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {
    IChannelAggregateRoot *channel = it->second;
    ChannelClientList &clientList = channel->getListConnects();

    if (clientList.getClients().empty()) {
      this->_logger->debugss() << "Channel " << channel->getName() << " is empty, removing it";
      this->_channelDB->remove(channel->getName());
    }
  }
  // CLIENT_DISCONNECT にすれば後の処理で自動的に切断される。
  client->setClientType(CLIENT_DISCONNECT);
  // _clientDBからクライアントを削除する手続きはRemoveConnectionUseCase::removeで行われる
  return SendMsgDTO(0, messageStreams);
}
