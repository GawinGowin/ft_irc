#include "domain/message/Message.hpp"
#include "Parser.hpp"

Message::Message() {
  this->_prefix = "";
  this->_command = IMessageAggregateRoot::UNKNOWN;
  this->_params = std::vector<std::string>();
}

Message::Message(const std::string &message) {
  Parser parser(message);
  this->_prefix = parser.getPrefix();
  this->_command = parser.getCommand();
  this->_params = parser.getParams();
}

Message::~Message() {}

Message::Message(const Message &obj) { *this = obj; }

Message &Message::operator=(const Message &obj) {
  if (this != &obj) {
    this->_prefix = obj._prefix;
    this->_command = obj._command;
    this->_params = obj._params;
  }
  return *this;
}

const std::string &Message::getPrefix() const { return this->_prefix; }

const IMessageAggregateRoot::CommandType &Message::getCommand() const { return this->_command; }

const std::vector<std::string> &Message::getParams() const { return this->_params; }
