#include "application/useCases/SendMsgFromServerUseCase.hpp"

void SendMsgFromServerUseCase::send(IClientAggregateRoot *client, const SendMsgDTO &message) {
  MultiLogger *logger = LoggerServiceLocator::get();
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  if (client == NULL) {
    logger->errorss() << "Failed to send message to client: client is NULL";
    return;
  }
  MessageStream ss = MessageService::generateMessageStream(_socketHandler, client);
  ss << message << std::endl;
  int status = ss.send();
  int fd = client->getSocketFd();
  if (status == -1) {
    logger->errorss() << "Failed to send message to client: " << client->getAddress()
                      << " (fd: " << fd << ")";
  } else {
    logger->tracess() << "Message sent to client: " << client->getAddress() << " (fd: " << fd << ")"
                      << message;
  }
}
