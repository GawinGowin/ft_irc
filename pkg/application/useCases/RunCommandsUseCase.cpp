#include "application/useCases/RunCommandsUseCase.hpp"

SendMsgDTO RunCommandsUseCase::execute(RecievedMsgDTO &recieved) {
  MultiLogger *logger = LoggerServiceLocator::get();
  Message clientMsg;
  IClientAggregateRoot *client = recieved.getClient();
  SendMsgDTO dto;

  const std::string &msg = recieved.getMessage();

  if (msg.empty()) {
    logger->warningss() << "Recieved message is empty (fd: " << client->getSocketFd() << ")";
  } else {
    clientMsg = Message(msg);
    logger->tracess() << "Recieved message: " << clientMsg;
  }
  switch (clientMsg.getCommand()) {
  case (MessageConstants::PASS):
    dto = Pass(&clientMsg, client).execute();
    break;
  case (MessageConstants::NICK):
    dto = Nick(&clientMsg, client).execute();
    break;
  case (MessageConstants::USER):
    dto = User(&clientMsg, client).execute();
    break;
  case (MessageConstants::JOIN):
    dto = Join(&clientMsg, client).execute();
    break;
  case (MessageConstants::PRIVMSG):
    dto = Privmsg(&clientMsg, client).execute();
    break;
  case (MessageConstants::KICK):
    dto = Kick(&clientMsg, client).execute();
    break;
  case (MessageConstants::INVITE):
    dto = Invite(&clientMsg, client).execute();
    break;
  case (MessageConstants::TOPIC):
    dto = Topic(&clientMsg, client).execute();
    break;
  case (MessageConstants::MODE):
    dto = Mode(&clientMsg, client).execute();
    break;
  case (MessageConstants::PING):
    dto = Ping(&clientMsg, client).execute();
    break;
  case (MessageConstants::PONG):
    dto = Pong(&clientMsg, client).execute();
    break;
  case (MessageConstants::QUIT):
  case (MessageConstants::UNDEFINED):
    dto = Quit(&clientMsg, client).execute();
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
