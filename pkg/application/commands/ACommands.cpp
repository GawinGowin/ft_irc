#include "application/commands/ACommands.hpp"

ACommands::ACommands() : _message(NULL), _client(NULL) {}

ACommands::ACommands(IMessageAggregateRoot *msg, IClientAggregateRoot *client)
    : _message(msg), _client(client) {}

ACommands::~ACommands() {}

ACommands::ACommands(const ACommands &obj) { *this = obj; }

ACommands &ACommands::operator=(const ACommands &obj) {
  if (this != &obj) {
    this->_message = obj._message;
    this->_client = obj._client;
  }
  return *this;
}

IMessageAggregateRoot *ACommands::getMessage() const { return this->_message; }

IClientAggregateRoot *ACommands::getClient() const { return this->_client; }
