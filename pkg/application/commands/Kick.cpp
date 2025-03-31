#include "application/commands/Kick.hpp"

Kick::Kick(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_channelDB = &InmemoryChannelDBServiceLocator::get();
  this->_clientDB = &InmemoryClientDBServiceLocator::get();
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
  this->_socketHandler = &SocketHandlerServiceLocator::get();
}

SendMsgDTO Kick::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();

  MessageStreamVector streams;
  const std::string &serverName = this->_conf->getConfigs().Global.Name;

  if (ClientService::login(*client) != ClientService::LOGIN_ALREADY) {
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
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " KICK :Not enough parameters");
    streams.push_back(stream);
    this->_logger->debugss() << "[KICK]: Not enough parameters (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }

  const std::string &channelName = msg->getParams()[0];
  const std::string &nickname = msg->getParams()[1];
  const std::string &reason = msg->getParams().size() > 2 ? msg->getParams()[2] : nickname;

  IChannelAggregateRoot *channel = this->_channelDB->get(channelName);
  if (channel == NULL) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOSUCHCHANNEL,
        client->getNickName() + " " + channelName + " :No such channel");
    streams.push_back(stream);
    this->_logger->debugss() << "[KICK]: No such channel " << channelName << " ("
                             << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }

  if (!channel->isOperator(client->getNickName())) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_CHANOPRIVSNEEDED,
        client->getNickName() + " " + channelName + " :You're not channel operator");
    streams.push_back(stream);
    this->_logger->debugss() << "[KICK]: Not channel operator " << channelName << " ("
                             << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }

  if (!channel->getListConnects().isClientInList(nickname)) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_USERNOTINCHANNEL,
        client->getNickName() + " " + nickname + " " + channelName +
            " :They aren't on that channel");
    streams.push_back(stream);
    this->_logger->debugss() << "[KICK]: User " << nickname << " not in channel " << channelName
                             << " (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }

  // KICKメッセージ送信
  const std::string &message = nickname + " :" + reason;

  std::stringstream ss;
  ss << Message(
      client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
      MessageConstants::KICK, channelName + " " + message);
  MessageStreamVector channelStreams = MessageService::generateMessageToChannel(
      this->_socketHandler, client, this->_clientDB, channel, ss.str());
  streams.insert(streams.end(), channelStreams.begin(), channelStreams.end());
  MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
  streams.push_back(stream << ss.str());
  channel->getListConnects().removeClient(nickname);
  this->_logger->infoss() << "[KICK]: User " << nickname << " kicked from " << channelName << " by "
                          << client->getNickName();
  return SendMsgDTO(0, streams);
}
