#include "application/useCases/SendMsgFromServerUseCase.hpp"

void SendMsgFromServerUseCase::send(SendMsgDTO &message) {
  MultiLogger *logger = LoggerServiceLocator::get();
  MessageStreamVector &msgStreams = message.getMessageStreams();

  std::vector<int> results = msgStreams.send();

  for (size_t i = 0; i < results.size(); ++i) {
    IClientAggregateRoot *client = msgStreams[i].getClient();
    if (results[i] != 0) {
      logger->errorss() << "Failed to send message to " << client->getNickName();
    } else {
      logger->tracess() << "Success to send message to " << client->getNickName();
    }
  }
}
