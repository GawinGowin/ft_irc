#ifndef ACOMMANDS_HPP
#define ACOMMANDS_HPP

#include "application/dto/RecievedMsgDTO.hpp"
#include "application/dto/SendMsgDTO.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/Message.hpp"

#include <string>

class ACommands {
public:
  ACommands();
  ACommands(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  virtual ~ACommands();
  ACommands(const ACommands &obj);
  ACommands &operator=(const ACommands &obj);

  virtual SendMsgDTO execute() = 0;

protected:
  IMessageAggregateRoot *getMessage() const;
  IClientAggregateRoot *getClient() const;

private:
  IClientAggregateRoot *_client;
  IMessageAggregateRoot *_message;
};

#endif /* ACOMMANDS_HPP */
