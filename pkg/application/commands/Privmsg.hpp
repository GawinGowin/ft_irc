#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

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

class Privmsg : public ACommands {
public:
  Privmsg(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  SendMsgDTO _handleChannelMessage(const std::string &channelName, const std::string &message);
  SendMsgDTO _handleUserMessage(const std::string &nickname, const std::string &message);

  InmemoryChannelDatabase *_channelDB;
  InmemoryClientDatabase *_clientDB;
  MultiLogger *_logger;
  ConfigsLoader *_conf;
  ISocketHandler *_socketHandler;
};

#endif /* PRIVMSG_HPP */
