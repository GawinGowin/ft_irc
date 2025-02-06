#ifndef ACOMMANDS_HPP
#define ACOMMANDS_HPP

#include "application/dto/RecievedMsgDTO.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/BaseMessage.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"

#include <string>

class ACommands {
public:
  ACommands();
  ACommands(const RecievedMsgDTO &message);
  virtual ~ACommands();
  ACommands(const ACommands &obj);
  ACommands &operator=(const ACommands &obj);

  virtual void execute(IClientAggregateRoot &client) = 0;

protected:
  const BaseMessage &getMessage() const;

private:
  BaseMessage *_parseMessage(const RecievedMsgDTO &message);
  BaseMessage *_message;
};

#endif /* ACOMMANDS_HPP */
