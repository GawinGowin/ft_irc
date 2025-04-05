#include "domain/client/ClientService.hpp"

ClientService::LoginResult ClientService::login(IClientAggregateRoot &client) {
  if (client.getClientType() == CLIENT_USER) {
    return ClientService::LOGIN_ALREADY;
  }

  bool hasPass = client.getClientType() & CLIENT_GOTPASS;
  bool hasNick = client.getClientType() & CLIENT_GOTNICK;
  bool hasUser = client.getClientType() & CLIENT_GOTUSER;

  ConfigsLoader &conf = ConfigsServiceLocator::get();

  const std::string &challengePass = client.getPassword();
  const std::string &svrPass = conf.getConfigs().Global.Password;

  bool passValid = (challengePass == svrPass);

  if (hasNick && hasUser) {
    if (hasPass && passValid) {
      client.setClientType(CLIENT_USER);
      return ClientService::LOGIN_SUCCESS;
    } else {
      client.setClientType(CLIENT_DISCONNECT);
      return ClientService::LOGIN_FAILED;
    }
  }
  return ClientService::LOGIN_SKIP;
}

std::string ClientService::generateWelcomeMessage(IClientAggregateRoot &client) {
  return "Welcome to the Internet Relay Network " + client.getNickName() + "!" +
         client.getUserName() + "@" + client.getAddress();
}

void ClientService::writeWelcomeMessageToStream(
    MessageStream &stream, IClientAggregateRoot &client) {
  stream << Message(
      ConfigsServiceLocator::get().getConfigs().Global.Name,
      MessageConstants::ResponseCode::RPL_WELCOME,
      client.getNickName() + " :" + ClientService::generateWelcomeMessage(client));
  stream << Message(
      ConfigsServiceLocator::get().getConfigs().Global.Name,
      MessageConstants::ResponseCode::RPL_YOURHOST,
      client.getNickName() + " :Your host is " +
          ConfigsServiceLocator::get().getConfigs().Global.Name + ", running version " +
          ConfigsServiceLocator::get().getConfigs().Global.Version);
  stream << Message(
      ConfigsServiceLocator::get().getConfigs().Global.Name,
      MessageConstants::ResponseCode::RPL_CREATED,
      client.getNickName() + " :This server was created " +
          ConfigsServiceLocator::get().getConfigs().Global.StartStr);
  stream << Message(
      ConfigsServiceLocator::get().getConfigs().Global.Name,
      MessageConstants::ResponseCode::RPL_MYINFO,
      client.getNickName() + " " + ConfigsServiceLocator::get().getConfigs().Global.Name + " " +
          ConfigsServiceLocator::get().getConfigs().Global.Version + " " +
          ConfigsServiceLocator::get().getConfigs().Global.UserModes + " " +
          ConfigsServiceLocator::get().getConfigs().Global.ChanModes);
}
