#include "application/commands/Ping.hpp"

Ping::Ping(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Ping::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;
  MultiLogger *logger = LoggerServiceLocator::get();

  // PING requires at least one parameter (token)
  if (msg->getParams().size() < 1) {
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "PING :Not enough parameters");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  // Get the token from the first parameter
  std::string token = msg->getParams()[0];
  
  // Get the target from the second parameter if it exists, otherwise use the server name
  std::string target = (msg->getParams().size() > 1) ? msg->getParams()[1] : serverName;

  // Log the PING request
  logger->debug("PING received from client " + client->getNickName() + " with token: " + token);

  // Send PONG response with the token
  stream << Message(serverName, MessageConstants::PONG, token + " " + target);
  messageStreams.push_back(stream);
  
  return SendMsgDTO(0, messageStreams);
}
