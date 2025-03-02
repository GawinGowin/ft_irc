#ifndef MODE_HPP
#define MODE_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"

class Mode : public ACommands {
public:
  Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();
};

#endif /* MODE_HPP */
