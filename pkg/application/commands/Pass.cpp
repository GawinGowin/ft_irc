#include "application/commands/Pass.hpp"

Pass::Pass(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Pass::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  // 2回目以降のPASSまたは、NICK/USER受信後のPASS
  if (client->getClientType() != CLIENT_UNKNOWN) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_ALREADYREGISTRED,
        "* ::Connection already registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  if (msg->getParams().size() != 1 || msg->getParams()[0].empty()) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "* PASS ::Syntax error");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  if (client->setPassword(msg->getParams()[0])) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_ALREADYREGISTRED,
        "* ::Connection already registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  client->setClientType(CLIENT_GOTPASS);
  return SendMsgDTO(0, messageStreams);
}
