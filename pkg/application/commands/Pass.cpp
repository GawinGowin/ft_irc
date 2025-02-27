#include "application/commands/Pass.hpp"

Pass::Pass(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Pass::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  SendMsgDTO dto;
  if (msg->getParams().size() != 1) {
    dto.setStatus(1);
    dto.setMessage(Message(":irc.example.net 461 * PASS :Syntax error")); // TODO: tmp
    logger->debugss() << "[PASS] by (fd: " << client->getSocketFd() << "): Syntax error";
    return dto;
  }
  if (client->setPassword(msg->getParams()[0])) {
    dto.setStatus(1);
    dto.setMessage(Message(":irc.example.net 462 * :Connection already registered")); // TODO: tmp
    logger->debugss() << "[PASS] by (fd: " << client->getSocketFd() << "): already registered";
    return dto;
  }
  logger->debugss() << "[PASS] by (fd: " << client->getSocketFd() << "): success";
  dto.setStatus(0);
  return dto;
}
