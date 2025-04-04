#include "application/commands/User.hpp"

static int checkUserName(const std::string &userName);

User::User(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
}

SendMsgDTO User::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if (!(client->getClientType() & CLIENT_GOTUSER)) {
    if (msg->getParams().size() != 4) {
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "* :Syntax error");
      messageStreams.push_back(stream);
      return SendMsgDTO(1, messageStreams);
    }
    if (checkUserName(msg->getParams()[0])) {
      client->setClientType(CLIENT_DISCONNECT);
      stream << Message(
          "", MessageConstants::ERROR,
          ":Closing connection: *[@" + client->getAddress() + "] (Invalid user name)\r\n");
      return SendMsgDTO(1, messageStreams);
    }
    // @が含まれてたらそれ以降切り捨て
    std::string userName = msg->getParams()[0];
    if (userName.find('@') != std::string::npos) {
      userName = userName.substr(0, userName.find('@'));
    }
    client->setUserName(userName);
    if (msg->getParams()[3].empty()) {
      client->setRealName("-");
    } else {
      client->setRealName(msg->getParams()[3]);
    }
    client->setClientType(client->getClientType() | CLIENT_GOTUSER);

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
      break;
    case ClientService::LOGIN_SKIP:
      break;
    }
    return SendMsgDTO(0, messageStreams);
  } else if (client->getClientType() & CLIENT_USER) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_ALREADYREGISTRED,
        "* :Connection already registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  } else {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
        "* :Connection not registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }
}

static int checkUserName(const std::string &userName) {
  for (size_t i = 0; i < userName.size(); i++) {
    if (!std::isalnum(userName[i]) && userName[i] != '+' && userName[i] != '-' &&
        userName[i] != '.' && userName[i] != '_' && userName[i] != '@') {
      return 1;
    }
  }
  return 0;
}
