#ifndef JOIN_HPP
#define JOIN_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "domain/channel/Channel.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"

class Join : public ACommands {
public:
  Join(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();
};

#endif /* JOIN_HPP */
