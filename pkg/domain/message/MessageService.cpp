#include "domain/message/MessageService.hpp"

MessageStream
MessageService::generateMessageStream(ISocketHandler *socketHandler, IClientAggregateRoot *client) {
  return MessageStream(socketHandler, client);
}

std::vector<MessageStream> MessageService::generateMessageToChannel(
    ISocketHandler *socketHandler,
    IClientAggregateRoot *sender,
    IClientRepository *clientRepository,
    IChannelAggregateRoot *ch,
    const std::string &msg) {

  std::vector<MessageStream> messageStreams;

  const std::vector<std::string> &members = ch->getListConnects().getClients();
  for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end(); ++it) {

    IClientAggregateRoot *member = clientRepository->getById(*it);

    if (member && member->getNickName() != sender->getNickName()) {
      MessageStream stream = generateMessageStream(socketHandler, member);
      stream << msg << "\r\n";
      messageStreams.push_back(stream);
    }
  }
  return (messageStreams);
}
