#include "application/commands/Nick.hpp"

Nick::Nick(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Nick::execute() {
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
  client->setNickName(msg->getParams()[0]);
  return SendMsgDTO(0, messageStreams);
}
