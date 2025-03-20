#include "application/commands/User.hpp"

User::User(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO User::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if (client->getClientType() & CLIENT_GOTUSER && client->getClientType() != CLIENT_USER) {
    stream << Message(serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
                      ":Connection not registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  (void)msg;
  (void)stream;
  (void)serverName;
  client->setClientType(CLIENT_GOTUSER);
  return SendMsgDTO(0, messageStreams);
}
