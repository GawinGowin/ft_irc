#include "domain/message/BaseMessage.hpp"
#include "Parser.hpp"

BaseMessage::BaseMessage() {
  this->_prefix = "";
  this->_command = IMessageAggregateRoot::UNKNOWN;
  this->_params = std::vector<std::string>();
}

BaseMessage::BaseMessage(const std::string &message) {
  Parser parser(message);
  this->_prefix = parser.getPrefix();
  this->_command = parser.getCommand();
  this->_params = parser.getParams();
}

BaseMessage::~BaseMessage() {}

BaseMessage::BaseMessage(const BaseMessage &obj) { *this = obj; }

BaseMessage &BaseMessage::operator=(const BaseMessage &obj) {
  if (this != &obj) {
    this->_prefix = obj._prefix;
    this->_command = obj._command;
    this->_params = obj._params;
  }
  return *this;
}

const std::string &BaseMessage::getPrefix() const { return this->_prefix; }

const IMessageAggregateRoot::CommandType &BaseMessage::getCommand() const { return this->_command; }

const std::vector<std::string> &BaseMessage::getParams() const { return this->_params; }
