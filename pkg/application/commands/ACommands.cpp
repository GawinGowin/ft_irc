#include "application/commands/ACommands.hpp"

ACommands::ACommands() : _message(NULL) {}

ACommands::ACommands(const RecievedMsgDTO &message) : _message(_parseMessage(message)) {}

ACommands::~ACommands() {
  if (_message != NULL) {
    delete _message;
  }
}

ACommands::ACommands(const ACommands &obj) { *this = obj; }

ACommands &ACommands::operator=(const ACommands &obj) {
  if (this != &obj) {
    if (_message != NULL) {
      delete _message;
    }
    _message = obj._message;
  }
  return *this;
}

const Message &ACommands::getMessage() const { return *_message; }

Message *ACommands::_parseMessage(const RecievedMsgDTO &message) {
  return new Message(message.getMessage());
}
