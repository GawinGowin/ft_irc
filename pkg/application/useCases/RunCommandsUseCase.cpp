#include "application/useCases/RunCommandsUseCase.hpp"

int RunCommandsUseCase::execute(const RecievedMsgDTO &recieved) {
  MultiLogger *logger = LoggerServiceLocator::get();
  if (recieved.getMessage().empty()) {
    logger->error("Recieved message is empty");
    throw std::invalid_argument("Recieved message is empty");
  }
  Message clientMsg(recieved.getMessage());
  Message serverResp;
  logger->tracess() << "clientMsg: " << clientMsg;
  switch (clientMsg.getCommand()) {
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
  logger->tracess() << "serverResp: " << serverResp;
  return (0);
}
