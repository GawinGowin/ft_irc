#ifndef QUIT_HPP
#define QUIT_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/channel/Channel.hpp"
#include "domain/channel/ChannelService.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"

class Quit : public ACommands {
public:
  Quit(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  InmemoryChannelDatabase *_channelDB;
  InmemoryClientDatabase *_clientDB;
  MultiLogger *_logger;
  ConfigsLoader *_conf;
  ISocketHandler *_socketHandler;
};

#endif /* QUIT_HPP */
