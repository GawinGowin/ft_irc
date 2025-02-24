#ifndef PASSCOMMAND_HPP
#define PASSCOMMAND_HPP

#include "application/commands/ACommands.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"

class PassCommand : public ACommands {
public:
  PassCommand(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  SendMsgDTO execute();
};

#endif /* PASSCOMMAND_HPP */
