#include "application/commands/Invite.hpp"

Invite::Invite(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_channelDB = &InmemoryChannelDBServiceLocator::get();
  this->_clientDB = &InmemoryClientDBServiceLocator::get();
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
  this->_socketHandler = &SocketHandlerServiceLocator::get();
}

SendMsgDTO Invite::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;
  if (msg->getParams().size() != 2) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " INVITE :Not enough parameters");
    messageStreams.push_back(stream);
    this->_logger->debugss() << "[INVITE]: Not enough parameters (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, messageStreams);
  }

  const std::string &targetNick = msg->getParams()[0];
  const std::string &channelName = msg->getParams()[1];

  IClientAggregateRoot *targetClient = this->_clientDB->getById(targetNick);
  if (!targetClient) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOSUCHNICK,
        client->getNickName() + " " + targetNick + " :No such nick/channel");
    messageStreams.push_back(stream);
    this->_logger->debugss() << "[INVITE]: No such nick (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, messageStreams);
  }
  IChannelAggregateRoot *channel = this->_channelDB->get(channelName);
  if (!channel) {
    // RFC2812では、チャンネルが存在しなくても招待は可能
    this->_logger->debugss() << "[INVITE]: Channel does not exist: " << channelName << " ("
                             << client->getSocketFd() << ")";
  } else {
    if (!channel->getListConnects().isClientInList(client->getNickName())) {
      MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_NOTONCHANNEL,
          client->getNickName() + " " + channelName + " :You are not on that channel");
      messageStreams.push_back(stream);
      this->_logger->debugss() << "[INVITE]: Not on channel (" << client->getSocketFd() << ")";
      return SendMsgDTO(1, messageStreams);
    }

    // チャンネルモード「+i」（招待のみ）が設定されている場合、オペレーター権限の確認
    if (channel->isInviteOnly() && !channel->isOperator(client->getNickName())) {
      MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_CHANOPRIVSNEEDED,
          client->getNickName() + " " + channelName + " :You're not channel operator");
      messageStreams.push_back(stream);
      this->_logger->debugss() << "[INVITE]: Not channel operator (" << client->getSocketFd()
                               << ")";
      return SendMsgDTO(1, messageStreams);
    }

    // 対象ユーザーが既にチャンネルのメンバーである場合
    if (channel->getListConnects().isClientInList(targetNick)) {
      MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_USERONCHANNEL,
          client->getNickName() + " " + targetNick + " " + channelName + " :is already on channel");
      messageStreams.push_back(stream);
      this->_logger->debugss() << "[INVITE]: User already on channel (" << client->getSocketFd()
                               << ")";
      return SendMsgDTO(1, messageStreams);
    }
    channel->getListInvites().addClient(targetNick);
  }

  MessageStream inviterStream = MessageService::generateMessageStream(this->_socketHandler, client);
  inviterStream << Message(
      serverName, MessageConstants::ResponseCode::RPL_INVITING,
      client->getNickName() + " " + targetNick + " " + channelName);
  messageStreams.push_back(inviterStream);

  MessageStream targetStream =
      MessageService::generateMessageStream(this->_socketHandler, targetClient);
  targetStream << Message(
      client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
      MessageConstants::INVITE, targetNick + " " + channelName);
  messageStreams.push_back(targetStream);

  this->_logger->debugss() << "[INVITE]: " << client->getNickName() << " invited " << targetNick
                           << " to " << channelName;
  return SendMsgDTO(0, messageStreams);
}