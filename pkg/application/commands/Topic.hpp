#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"
#include "application/dto/SendMsgDTO.hpp"

class Topic : public ACommands {
public:
  Topic(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  SendMsgDTO showTopic(IChannelAggregateRoot *channel);
  SendMsgDTO setTopic(IChannelAggregateRoot *channel, const std::string &topic);
};

#endif /* TOPIC_HPP */
