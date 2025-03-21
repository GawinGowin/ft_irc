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
    stream << Message(
        serverName, MessageConstants::ResponseCode::RPL_WELCOME,
        client->getNickName() + " :Welcome to the Internet Relay Network " + client->getNickName() +
            "! " + client->getUserName() + "@" + client->getAddress());
    messageStreams.push_back(stream);
  } else if (client->getClientType() == CLIENT_NONPASS) {
    client->setClientType(CLIENT_DISCONNECT);
    stream << "ERROR :Closing connection: " + client->getNickName() + "[" + client->getUserName() +
                  "@" + client->getAddress() + "] (Access denied: Bad password?)\r\n";
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  return SendMsgDTO(0, messageStreams);
}
