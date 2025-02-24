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
  ACommands(const RecievedMsgDTO &message);
  virtual ~ACommands();
  ACommands(const ACommands &obj);
  ACommands &operator=(const ACommands &obj);

  virtual SendMsgDTO execute(IClientAggregateRoot &client) = 0;

protected:
  const Message &getMessage() const;

private:
  Message *_parseMessage(const RecievedMsgDTO &message);
  Message *_message;
};

#endif /* ACOMMANDS_HPP */
