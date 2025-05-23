#ifndef INVITE_HPP
#define INVITE_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/channel/Channel.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"

class Invite : public ACommands {
public:
  Invite(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  InmemoryChannelDatabase *_channelDB;
  InmemoryClientDatabase *_clientDB;
  MultiLogger *_logger;
  ConfigsLoader *_conf;
  ISocketHandler *_socketHandler;
};

#endif /* INVITE_HPP */
