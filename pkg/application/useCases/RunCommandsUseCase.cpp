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
  case (MessageConstants::PASS):
    dto = Pass(&clientMsg, client).execute();
    break;
  case (MessageConstants::NICK): {
    Nick nick(&clientMsg, client);
    dto = nick.execute();
    break;
  }
  case (MessageConstants::USER):
    // dto = ..
    break;
  case (MessageConstants::JOIN):
    dto = Join(&clientMsg, client).execute();
    break;
  case (MessageConstants::PRIVMSG):
    // dto = ..
    break;
  case (MessageConstants::KICK):
    // dto = ..
    break;
  case (MessageConstants::INVITE):
    // dto = ..
    break;
  case (MessageConstants::TOPIC):
    // dto = ..
    break;
  case (MessageConstants::MODE):
    dto = Mode(&clientMsg, client).execute();
    break;
  case (MessageConstants::ERROR):
    dto.setStatus(1);
    return (dto);
  case (MessageConstants::UNKNOWN):
    break;
  default:
    break;
  }
  return (dto);
}
