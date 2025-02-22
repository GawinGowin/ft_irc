#include "application/useCases/RunCommandsUseCase.hpp"

int RunCommandsUseCase::execute(const RecievedMsgDTO &recieved) {
  if (recieved.getMessage().empty()) {
    throw std::invalid_argument("Recieved message is empty");
  }
  Message msg(recieved.getMessage());
  switch (msg.getCommand()) {
  case (IMessageAggregateRoot::PASS):
    /* code */
    break;
  case (IMessageAggregateRoot::NICK):
    /* code */
    break;
  case (IMessageAggregateRoot::USER):
    /* code */
    break;
  case (IMessageAggregateRoot::JOIN):
    /* code */
    break;
  case (IMessageAggregateRoot::PRIVMSG):
    /* code */
    break;
  case (IMessageAggregateRoot::KICK):
    /* code */
    break;
  case (IMessageAggregateRoot::INVITE):
    /* code */
    break;
  case (IMessageAggregateRoot::TOPIC):
    /* code */
    break;
  case (IMessageAggregateRoot::MODE):
    /* code */
    break;
  case (IMessageAggregateRoot::ERROR):
    return (1);
  case (IMessageAggregateRoot::UNKNOWN):
    break;
  default:
    break;
  }
  return (0);
}
