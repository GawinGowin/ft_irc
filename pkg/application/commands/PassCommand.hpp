#ifndef PASSCOMMAND_HPP
#define PASSCOMMAND_HPP

#include "application/commands/ACommands.hpp"

class PassCommand : public ACommands {
public:
  PassCommand(const RecievedMsgDTO &message);
  virtual void execute(IClientAggregateRoot &client) override;
};

#endif /* PASSCOMMAND_HPP */
