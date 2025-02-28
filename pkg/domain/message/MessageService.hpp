#ifndef MESSAGESERVICE_HPP
#define MESSAGESERVICE_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/client/IClientRepository.hpp"
#include "domain/message/MessageStream.hpp"
#include "infra/socket/ISocketHandler.hpp"

#include <vector>

class MessageService {
public:
  static MessageStream
  generateMessageStream(ISocketHandler *socketHandler, IClientAggregateRoot *client);

  static std::vector<MessageStream> generateMessageToChannel(
      ISocketHandler *,
      IClientAggregateRoot *,
      IClientRepository *,
      IChannelAggregateRoot *,
      const std::string &);
};

#endif /* MESSAGESERVICE_HPP */
