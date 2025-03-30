#ifndef PONG_HPP
#define PONG_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"

class Pong : public ACommands {
public:
  Pong(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();
};

#endif /* PONG_HPP */
