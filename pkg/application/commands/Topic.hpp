#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "application/commands/ACommands.hpp"
#include "application/dto/SendMsgDTO.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/client/ClientService.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"
#include "domain/message/MessageStreamVector.hpp"
#include "infra/database/InmemoryChannelDatabase.hpp"
#include "infra/socket/ISocketHandler.hpp"

class Topic : public ACommands {
public:
  Topic(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  SendMsgDTO showTopic(IChannelAggregateRoot *channel);
  SendMsgDTO setTopic(IChannelAggregateRoot *channel, const std::string &topic);
};

#endif /* TOPIC_HPP */
