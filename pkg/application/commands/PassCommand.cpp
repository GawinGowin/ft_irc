#include "application/commands/PassCommand.hpp"

PassCommand::PassCommand(IMessageAggregateRoot *msg, IClientAggregateRoot *client)
    : ACommands(msg, client) {}

SendMsgDTO PassCommand::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  SendMsgDTO dto;
  if (msg->getParams().size() != 1) {
    dto.setStatus(1);
    dto.setMessage(Message(":irc.example.net 461 * PASS :Syntax error")); // TODO: tmp
    return dto;
  }
  if (!client->getPassword().empty()) {
    dto.setStatus(1);
    dto.setMessage(Message(":irc.example.net 462 * :Connection already registered")); // TODO: tmp
    return dto;
  }
  client->setPassword(msg->getParams()[0]);
  logger->debugss() << "[PASS] by (fd: " << client->getSocketFd() << ")";
  return dto;
}
