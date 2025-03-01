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
  case (CommandType::PASS):
    dto = Pass(&clientMsg, client).execute();
    break;
  case (CommandType::NICK): {
    Nick nick(&clientMsg, client);
    dto = nick.execute();
    break;
  }
  case (CommandType::USER):
    // dto = ..
    break;
  case (CommandType::JOIN):
    dto = Join(&clientMsg, client).execute();
    break;
  case (CommandType::PRIVMSG):
    // dto = ..
    break;
  case (CommandType::KICK):
    // dto = ..
    break;
  case (CommandType::INVITE):
    // dto = ..
    break;
  case (CommandType::TOPIC):
    // dto = ..
    break;
  case (CommandType::MODE):
    // dto = ..
    break;
  case (CommandType::ERROR):
    dto.setStatus(1);
    return (dto);
  case (CommandType::UNKNOWN):
    break;
  default:
    break;
  }
  logger->tracess() << "serverResp: " << dto;
  return (dto);
}
