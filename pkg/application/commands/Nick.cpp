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
  client->setClientType(CLIENT_GOTNICK);
  if (client->getClientType() == CLIENT_LOGIN) {
    client->setClientType(CLIENT_USER);
  } else if (client->getClientType() == CLIENT_NONPASS) {
    // ERROR :Closing connection: sya[~g@172.18.0.1] (Access denied: Bad password?)
    client->setClientType(CLIENT_DISCONNECT);
    return SendMsgDTO(1, messageStreams);
  }
  return SendMsgDTO(0, messageStreams);
}
