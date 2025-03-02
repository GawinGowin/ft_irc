#include "application/commands/Pass.hpp"

Pass::Pass(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Pass::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if (msg->getParams().size() != 1) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "* :Syntax error");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  if (client->setPassword(msg->getParams()[0])) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_ALREADYREGISTRED,
        "* :Connection already registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  return SendMsgDTO(1, messageStreams);
}
