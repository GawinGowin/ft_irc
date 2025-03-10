#ifndef MODE_HPP
#define MODE_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"

#include <sstream>

class Mode : public ACommands {
public:
  Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();

private:
  SendMsgDTO _handleChannelMode(ISocketHandler &socketHandler, InmemoryChannelDatabase &channelDB);
  SendMsgDTO _handleUserMode(ISocketHandler &socketHandler, InmemoryChannelDatabase &channelDB);
  bool _is_channelOperator(const std::string &channel, const std::string &nickname);
};

#endif /* MODE_HPP */
