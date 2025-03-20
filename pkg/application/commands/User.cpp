#include "application/commands/User.hpp"

static int checkUserName(const std::string &userName);

User::User(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO User::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if ((client->getClientType() & CLIENT_GOTNICK || client->getClientType() & CLIENT_GOTPASS) && !(client->getClientType() & CLIENT_GOTUSER)) {
    if (msg->getParams().size() != 4) {
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "* :Syntax error");
      messageStreams.push_back(stream);
      return SendMsgDTO(1, messageStreams);
    }
    if (checkUserName(msg->getParams()[0])) {
      // ERROR :Closing connection: *[~@172.18.0.1] (Invalid user name)
      client->setClientType(CLIENT_DISCONNECT);
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
    
    client->setClientType(CLIENT_GOTUSER);
    if (client->getClientType() == CLIENT_LOGIN) {
      client->setClientType(CLIENT_USER);
    } else if (client->getClientType() == CLIENT_NONPASS) {
      // ERROR :Closing connection: sya[~g@172.18.0.1] (Access denied: Bad password?)
      client->setClientType(CLIENT_DISCONNECT);
      return SendMsgDTO(1, messageStreams);
    }
    return SendMsgDTO(0, messageStreams);
  } else if (client->getClientType() & CLIENT_USER) {
    stream << Message(serverName, MessageConstants::ResponseCode::ERR_ALREADYREGISTRED,
                      "* :Connection already registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);  
  } else {
    stream << Message(serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
                      "* :Connection not registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);  
  }
}

static int checkUserName(const std::string &userName) {
  for (size_t i = 0; i < userName.size(); i++) {
    if (!std::isalnum(userName[i]) && userName[i] != '+' && userName[i] != '-' && userName[i] != '.' && userName[i] != '_' && userName[i] != '@') {
      return 1;
    }
  }
  return 0;
}
