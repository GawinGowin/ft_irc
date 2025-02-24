#include "application/useCases/RunCommandsUseCase.hpp"

SendMsgDTO RunCommandsUseCase::execute(const RecievedMsgDTO &recieved) {
  MultiLogger *logger = LoggerServiceLocator::get();
  if (recieved.getMessage().empty()) {
    logger->error("Recieved message is empty");
    throw std::invalid_argument("Recieved message is empty");
  }
  Message clientMsg(recieved.getMessage());
  Message serverResp;
  logger->tracess() << "clientMsg: " << clientMsg;

  SendMsgDTO dto;
  switch (clientMsg.getCommand()) {
  case (IMessageAggregateRoot::PASS):
    // dto = ..
    break;
  case (IMessageAggregateRoot::NICK):
    // dto = ..
    break;
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
  /* 以下の内容は仮置き */
  serverResp = Message(recieved.getMessage());
  logger->tracess() << "serverResp: " << serverResp;
  dto.setMessage(serverResp);
  /* 仮置き終わり */
  return (dto);
}
