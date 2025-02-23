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
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::NICK):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::USER):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::JOIN):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::PRIVMSG):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::KICK):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::INVITE):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::TOPIC):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::MODE):
    // serverResp = ..
    break;
  case (IMessageAggregateRoot::ERROR):
    dto.setStatus(1);
    return (dto);
  case (IMessageAggregateRoot::UNKNOWN):
    break;
  default:
    break;
  }
  serverResp = Message(recieved.getMessage());
  logger->tracess() << "serverResp: " << serverResp;
  dto.setMessage(serverResp);
  return (dto);
}
