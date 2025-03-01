#include "application/commands/Nick.hpp"

Nick::Nick() : ACommands() {}

Nick::Nick(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

Nick::~Nick() {}

Nick::Nick(const Nick &obj) : ACommands(obj) {}

Nick &Nick::operator=(const Nick &obj) {
  if (this == &obj) {
    return (*this);
  }
  ACommands::operator=(obj);
  return (*this);
}

SendMsgDTO Nick::execute() {
  SendMsgDTO dto;
  IMessageAggregateRoot &message = *getMessage();

  if (message.getParams().size() != 1) {
    dto.setStatus(1);
    return (dto);
  }

  _setNickName(message.getParams().at(0));
  return (dto);
}

void Nick::_setNickName(std::string nick) {
  IClientAggregateRoot &client = *getClient();
  client.setNickName(nick);
}
