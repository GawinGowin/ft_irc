#ifndef CLIENTSERVICE_HPP
#define CLIENTSERVICE_HPP

#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/client/Password.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/Message.hpp"
#include "domain/message/MessageService.hpp"

#include <string>

class ClientService {
public:
  enum LoginResult {
    LOGIN_SUCCESS = 0,
    LOGIN_FAILED = 1,
    LOGIN_ALREADY = 2,
    LOGIN_SKIP = 3,
  };
  static LoginResult login(IClientAggregateRoot &client);
  static std::string generateWelcomeMessage(IClientAggregateRoot &client);
  static void writeWelcomeMessageToStream(MessageStream &stream, IClientAggregateRoot &client);
};

#endif /* CLIENTSERVICE_HPP */
