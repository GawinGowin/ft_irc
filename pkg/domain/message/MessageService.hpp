#ifndef MESSAGESERVICE_HPP
#define MESSAGESERVICE_HPP

#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/MessageStream.hpp"
#include "infra/socket/ISocketHandler.hpp"

class MessageService {
public:
  static MessageStream generateMessageStream(ISocketHandler *, IClientAggregateRoot *);
};

#endif /* MESSAGESERVICE_HPP */
