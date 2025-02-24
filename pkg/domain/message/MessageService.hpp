#ifndef MESSAGESERVICE_HPP
#define MESSAGESERVICE_HPP

#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/MessageStream.hpp"
#include "infra/socket/ISocketHandler.hpp"

class MessageService {
public:
  static MessageStream
  generateMessageStream(ISocketHandler *socketHandler, IClientAggregateRoot *client) {
    return MessageStream(socketHandler, client);
  }
};

#endif /* MESSAGESERVICE_HPP */
