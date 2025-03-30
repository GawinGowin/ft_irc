#include "application/commands/Pong.hpp"

Pong::Pong(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Pong::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  
  MultiLogger *logger = LoggerServiceLocator::get();

  // Extract parameters
  std::string token = msg->getParams().size() > 0 ? msg->getParams()[0] : "";
  std::string target = msg->getParams().size() > 1 ? msg->getParams()[1] : "";

  // Log the PONG message
  logger->debug("PONG received from client " + client->getNickName() + 
                " with token: " + token + 
                (target.empty() ? "" : " and target: " + target));

  // PONG is just a response to PING, so we don't need to send anything back
  // Just log it and return success
  return SendMsgDTO(0, messageStreams);
}
