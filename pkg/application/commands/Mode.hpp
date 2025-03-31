#ifndef MODE_HPP
#define MODE_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/ClientService.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"

#include <set>
#include <sstream>

class Mode : public ACommands {
public:
  struct ModeChanges {
    int modeFlags;
    std::string newChannelKey;
    int newChannelLimit;
    std::set<std::string> newOperators;
    std::set<std::string> removedOperators;

    std::string ChangedFlags;
    std::string ChangedParams;
  };

  Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  SendMsgDTO _handleChannelMode(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO _handleUserMode(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  int _parseAndProcessChannelMode(
      ModeChanges *actualModeModifise,
      IChannelAggregateRoot *channel,
      const std::vector<std::string> &modeParams);

  int _check_invalid_nick_arg(std::string targetNick, IChannelAggregateRoot *channel);

  InmemoryChannelDatabase *_channelDB;
  InmemoryClientDatabase *_clientDB;
  MultiLogger *_logger;
  ConfigsLoader *_conf;
  ISocketHandler *_socketHandler;
};

#endif /* MODE_HPP */
