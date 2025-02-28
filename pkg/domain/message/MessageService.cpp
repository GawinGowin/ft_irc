#include "domain/message/MessageService.hpp"

MessageStream MessageService::generateMessageStream(ISocketHandler *s, IClientAggregateRoot *c) {
  return MessageStream(s, c);
}

