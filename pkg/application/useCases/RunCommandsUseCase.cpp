#include "application/useCases/RunCommandsUseCase.hpp"

SendMsgDTO RunCommandsUseCase::execute(RecievedMsgDTO &recieved) {
  MultiLogger *logger = LoggerServiceLocator::get();
  if (recieved.getMessage().empty()) {
    logger->error("Recieved message is empty");
    throw std::invalid_argument("Recieved message is empty");
  }
  Message clientMsg(recieved.getMessage());
  Message serverResp;
  logger->tracess() << "clientMsg: " << clientMsg;

  SendMsgDTO dto;
  IClientAggregateRoot *client = recieved.getClient();
  switch (clientMsg.getCommand()) {
  case (IMessageAggregateRoot::PASS):
    dto = Pass(&clientMsg, client).execute();
    break;
  case (IMessageAggregateRoot::NICK): {
    Nick nick(&clientMsg, client);
    dto = nick.execute();
    break;
  }
  case (IMessageAggregateRoot::USER):
    // dto = ..
    break;
  case (IMessageAggregateRoot::JOIN):
    // dto = ..
    break;
  case (IMessageAggregateRoot::PRIVMSG):
    // dto = ..
    break;
  case (IMessageAggregateRoot::KICK):
    // dto = ..
    break;
  case (IMessageAggregateRoot::INVITE):
    // dto = ..
    break;
  case (IMessageAggregateRoot::TOPIC):
    // dto = ..
    break;
  case (IMessageAggregateRoot::MODE):
    // dto = ..
    break;
  case (IMessageAggregateRoot::ERROR):
    dto.setStatus(1);
    return (dto);
  case (IMessageAggregateRoot::UNKNOWN):
    break;
  default:
    break;
  }
  logger->tracess() << "serverResp: " << dto;
  return (dto);
}
