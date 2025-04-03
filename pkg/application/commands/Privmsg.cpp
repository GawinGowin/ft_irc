#include "application/commands/Privmsg.hpp"

Privmsg::Privmsg(IMessageAggregateRoot *msg, IClientAggregateRoot *client)
    : ACommands(msg, client) {
  this->_channelDB = &InmemoryChannelDBServiceLocator::get();
  this->_clientDB = &InmemoryClientDBServiceLocator::get();
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
  this->_socketHandler = &SocketHandlerServiceLocator::get();
}

SendMsgDTO Privmsg::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();

  MessageStreamVector streams;

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;
  if (client->getClientType() != CLIENT_USER) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
        "* :Connection not registered");
    streams.push_back(stream);
    return SendMsgDTO(1, streams);
  }
  if (msg->getParams().size() < 2) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);

    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " PRIVMSG :Not enough parameters");
    streams.push_back(stream);
    this->_logger->debugss() << "[PRIVMSG]: Not enough parameters (" << client->getSocketFd()
                             << ")";
    return SendMsgDTO(1, streams);
  }

  const std::string &target = msg->getParams()[0];
  const std::string &message = msg->getParams()[1];
  if (this->_conf->getConfigs().Options.AllowedChannelTypes.find(target[0]) !=
      std::string::npos) { // 引数が"#"もしくは"&"で始まる場合
    return _handleChannelMessage(target, message);
  } else {
    return _handleUserMessage(target, message);
  }
}

SendMsgDTO
Privmsg::_handleChannelMessage(const std::string &channelName, const std::string &message) {
  MessageStreamVector streams;
  IClientAggregateRoot *client = this->getClient();

  IChannelAggregateRoot *channel = this->_channelDB->get(channelName);
  if (channel == NULL) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_NOSUCHCHANNEL,
        client->getNickName() + " " + channelName + " :No such channel");
    streams.push_back(stream);
    this->_logger->debugss() << "[PRIVMSG]: No such channel (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  // クライアントがチャンネルのメンバーである
  if (!channel->getListConnects().isClientInList(client->getNickName())) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_CANNOTSENDTOCHAN,
        client->getNickName() + " " + channelName + " :Cannot send to channel");
    streams.push_back(stream);
    this->_logger->debugss() << "[PRIVMSG]: Cannot send to channel (" << client->getSocketFd()
                             << ")";
    return SendMsgDTO(1, streams);
  }
  // メッセージ送信
  std::stringstream ss;
  ss << Message(
      client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
      MessageConstants::PRIVMSG, channelName + " :" + message);
  MessageStreamVector channelStreams = MessageService::generateMessageToChannel(
      this->_socketHandler, client, this->_clientDB, channel, ss.str());
  streams.insert(streams.end(), channelStreams.begin(), channelStreams.end());
  this->_logger->debugss() << "[PRIVMSG]: Message sent to channel " << channelName << " ("
                           << client->getSocketFd() << ")";
  return SendMsgDTO(0, streams);
}

SendMsgDTO Privmsg::_handleUserMessage(const std::string &nickname, const std::string &message) {
  MessageStreamVector streams;
  IClientAggregateRoot *client = this->getClient();

  IClientAggregateRoot *targetClient = this->_clientDB->getById(nickname);
  if (!targetClient) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_NOSUCHNICK,
        client->getNickName() + " " + nickname + " :No such nick/channel");
    streams.push_back(stream);
    this->_logger->debugss() << "[PRIVMSG]: No such nick (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }

  // メッセージを作成して送信
  MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, targetClient);
  stream << Message(
      client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
      MessageConstants::PRIVMSG, nickname + " :" + message);
  streams.push_back(stream);

  this->_logger->debugss() << "[PRIVMSG]: Message sent to " << nickname << " ("
                           << client->getSocketFd() << ")";
  return SendMsgDTO(0, streams);
}
