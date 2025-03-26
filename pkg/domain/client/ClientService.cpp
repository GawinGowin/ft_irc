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
