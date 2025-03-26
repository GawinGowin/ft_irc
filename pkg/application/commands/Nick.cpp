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

  const std::string nickName = msg->getParams()[0];

  if (InmemoryClientDBServiceLocator::get().getById(nickName) != NULL) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NICKNAMEINUSE,
        "* " + nickName + " ::Nickname already in use");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  client->setNickName(nickName);
  client->setClientType(CLIENT_GOTNICK);

  ClientService::LoginResult ret = ClientService::login(*client);
  switch (ret) {
  case ClientService::LOGIN_SUCCESS:
    stream << Message(
        serverName, MessageConstants::ResponseCode::RPL_WELCOME,
        client->getNickName() + " ::" + ClientService::generateWelcomeMessage(*client));
    messageStreams.push_back(stream);
    break;
  case ClientService::LOGIN_FAILED:
    stream << Message(
        "", MessageConstants::ERROR,
        "::Closing connection: " + client->getNickName() + "[" + client->getUserName() + "@" +
            client->getAddress() + "] (Access denied: Bad password?)");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
    break;
  case ClientService::LOGIN_ALREADY:
    stream << Message(
        client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
        MessageConstants::NICK, "::" + nickName);
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
    break;
  case ClientService::LOGIN_SKIP:
    break;
  }
  return SendMsgDTO(0, messageStreams);
}
