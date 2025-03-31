#include "application/commands/Pong.hpp"

Pong::Pong(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Pong::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;
  MultiLogger *logger = LoggerServiceLocator::get();

  std::string target = msg->getParams().size() > 0 ? msg->getParams()[0] : "";
  std::string token = msg->getParams().size() > 1 ? msg->getParams()[1] : "";

  if (target.empty()) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOORIGIN,
        client->getNickName() + " :No origin specified");
    messageStreams.push_back(stream);
    logger->trace("[PONG] received from client " + client->getNickName() + " but no target");
    return SendMsgDTO(1, messageStreams);
  }
  logger->debug(
      "[PONG] received from client " + client->getNickName() + " with token: " + token +
      (target.empty() ? "" : " and target: " + target));
  return SendMsgDTO(0, messageStreams);
}
