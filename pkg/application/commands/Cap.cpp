#include "application/commands/Cap.hpp"

Cap::Cap(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_channelDB = &InmemoryChannelDBServiceLocator::get();
  this->_clientDB = &InmemoryClientDBServiceLocator::get();
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
  this->_socketHandler = &SocketHandlerServiceLocator::get();
}

SendMsgDTO Cap::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);

  const std::string serverName = this->_conf->getConfigs().Global.Name;

  if (msg->getParams().size() < 1) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " CAP :Not enough parameters");
    messageStreams.push_back(stream);
    return (SendMsgDTO(1, messageStreams));
  }

  const std::string &subcommand = msg->getParams()[0];
  if (subcommand == "LS") {
    // 利用可能なケーパビリティは存在しないため、空のリストを返す
    stream << Message(
        serverName, MessageConstants::ResponseCode::RPL_CAP, client->getNickName() + " LS :");
    messageStreams.push_back(stream);
    return (SendMsgDTO(0, messageStreams));
  }

  // その他のサブコマンドは未実装
  stream << Message(
      serverName, MessageConstants::ResponseCode::ERR_INVALIDCAPCMD,
      client->getNickName() + " CAP :Invalid CAP subcommand");
  messageStreams.push_back(stream);
  return (SendMsgDTO(1, messageStreams));
}