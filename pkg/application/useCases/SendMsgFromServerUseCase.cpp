#include "application/useCases/SendMsgFromServerUseCase.hpp"

void SendMsgFromServerUseCase::send(Client &client, const SendMsgDTO &message) {
  MultiLogger *logger = LoggerServiceLocator::get();
  SocketHandler *_socketHandler = &SocketHandlerServiceLocator::get();
  MessageStream ss = MessageService::generateMessageStream(_socketHandler, &client);
  ss << message;
  int status = ss.send();
  if (status == -1) {
    logger->errorss() << "Failed to send message to client: " << client.getAddress()
                      << " (fd: " << client.getSocketFd() << ")";
  } else {
    logger->tracess() << "Message sent to client: " << client.getAddress()
                      << " (fd: " << client.getSocketFd() << ")";
    logger->tracess() << "Message: " << message;
  }
}
