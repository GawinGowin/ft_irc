#ifndef NICK_HPP
#define NICK_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"

class Nick : public ACommands {
public:
  Nick(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();
};

#endif /* NICK_HPP */
