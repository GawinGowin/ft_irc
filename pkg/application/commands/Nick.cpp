#include "application/commands/Nick.hpp"

static int checkNickName(const std::string &nickName);

Nick::Nick(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
}

SendMsgDTO Nick::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = this->_conf->getConfigs().Global.Name;

  if (msg->getParams().size() != 1) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "* NICK :Syntax error");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  const std::string nickName = msg->getParams()[0];
  const int maxNickLength = this->_conf->getConfigs().Limits.MaxNickLength;
  if (nickName.length() > INT32_MAX || static_cast<int>(nickName.length()) > maxNickLength) {
    std::stringstream ss;
    ss << "* " << nickName << " :Nickname too long, max. " << maxNickLength << " characters";
    stream << Message(serverName, MessageConstants::ResponseCode::ERR_NICKNAMETOOLONG, ss.str());
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  if (checkNickName(nickName)) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_ERRONEUSNICKNAME,
        "* " + nickName + " :Erroneous nickname");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
  if (InmemoryClientDBServiceLocator::get().getById(nickName) != NULL) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NICKNAMEINUSE,
        "* " + nickName + " :Nickname already in use");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  client->setNickName(nickName);
  client->setClientType(client->getClientType() | CLIENT_GOTNICK);

  ClientService::LoginResult ret = ClientService::login(*client);
  switch (ret) {
  case ClientService::LOGIN_SUCCESS:
    ClientService::writeWelcomeMessageToStream(stream, *client);
    messageStreams.push_back(stream);
    break;
  case ClientService::LOGIN_FAILED:
    stream << Message(
        "", MessageConstants::ERROR,
        ":Closing connection: " + client->getNickName() + "[" + client->getUserName() + "@" +
            client->getAddress() + "] (Access denied: Bad password?)");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
    break;
  case ClientService::LOGIN_ALREADY:
    stream << Message(
        client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
        MessageConstants::NICK, ":" + nickName);
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
    break;
  case ClientService::LOGIN_SKIP:
    break;
  }
  return SendMsgDTO(0, messageStreams);
}

static int checkNickName(const std::string &nickName) {
  for (size_t i = 0; i < nickName.size(); i++) {
    if (!std::isalnum(nickName[i]) || nickName[i] == '+' || nickName[i] == '-' ||
        nickName[i] == '.' || nickName[i] == '_' || nickName[i] == '@') {
      return 1;
    }
  }
  return 0;
}
