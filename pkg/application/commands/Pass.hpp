#ifndef PASS_HPP
#define PASS_HPP

#include "application/commands/ACommands.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"

class Pass : public ACommands {
public:
  Pass(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();
};

#endif /* PASS_HPP */
