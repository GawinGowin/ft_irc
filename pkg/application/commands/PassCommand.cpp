#include "application/commands/PassCommand.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/BaseMessage.hpp"
#include "application/response/Response.hpp"

PassCommand::PassCommand(const RecievedMsgDTO &message) : ACommands(message) {}

void PassCommand::execute(IClientAggregateRoot &client) {
  const BaseMessage &msg = getMessage();
  if (msg.getParams().empty()) {
    client.sendResponse(Response::ERR_NEEDMOREPARAMS("PASS"));
    return;
  }

  if (client.isRegistered()) {
    client.sendResponse(Response::ERR_ALREADYREGISTRED());
    return;
  }

  client.setPassword(msg.getParams()[0]);
}
