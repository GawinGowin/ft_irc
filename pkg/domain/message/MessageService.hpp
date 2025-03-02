#ifndef MESSAGESERVICE_HPP
#define MESSAGESERVICE_HPP

#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/client/IClientRepository.hpp"
#include "domain/message/MessageStream.hpp"
#include "domain/message/MessageStreamVector.hpp"
#include "infra/socket/ISocketHandler.hpp"

#include <vector>

class MessageService {
public:
  static MessageStream
  generateMessageStream(ISocketHandler *socketHandler, IClientAggregateRoot *client);

  static MessageStreamVector generateMessageToChannel(
      ISocketHandler *socketHandler,
      IClientAggregateRoot *sender,
      IClientRepository *clientRepository,
      IChannelAggregateRoot *ch,
      const std::string &msg);
};

#endif /* MESSAGESERVICE_HPP */
