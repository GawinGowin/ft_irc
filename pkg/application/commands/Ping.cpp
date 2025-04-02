#include "application/commands/Ping.hpp"

Ping::Ping(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Ping::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;
  MultiLogger *logger = LoggerServiceLocator::get();

  if (client->getClientType() != CLIENT_USER) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
        "* :Connection not registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  if (msg->getParams().size() < 1) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOORIGIN,
        client->getNickName() + " :No origin specified");
    messageStreams.push_back(stream);
    logger->trace("[PING] received from client " + client->getNickName() + " but no token");
    return SendMsgDTO(1, messageStreams);
  }

  std::string token = msg->getParams()[0];

  std::string target = (msg->getParams().size() > 1) ? msg->getParams()[1] : serverName;
  if (target != serverName) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOSUCHSERVER,
        client->getNickName() + " " + target + " :No such server");
    messageStreams.push_back(stream);
    logger->trace(
        "[PING] received from client " + client->getNickName() + " but target is invalid");
    return SendMsgDTO(1, messageStreams);
  }

  logger->debug("[PING] received from client " + client->getNickName() + " with token: " + token);
  stream << Message(serverName, MessageConstants::PONG, serverName + " :" + token);
  messageStreams.push_back(stream);
  return SendMsgDTO(0, messageStreams);
}
