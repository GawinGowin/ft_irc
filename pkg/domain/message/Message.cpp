#include "domain/message/Message.hpp"

std::ostream &operator<<(std::ostream &os, const Message &msg) {
  std::vector<std::string>::const_iterator it;
  os << "{prefix: \"" << msg.getPrefix()
     << "\", "
        "command: "
     << msg.getCommand() << ", params: [";
  for (it = msg.getParams().begin(); it != msg.getParams().end(); ++it) {
    os << "\"" << (*it) << "\"";
    if (it + 1 != msg.getParams().end())
      os << ", ";
  }
  os << "]}";
  return os;
}

Message::Message() {
  this->_prefix = "";
  this->_command = MessageConstants::UNDEFINED;
  this->_params = std::vector<std::string>();
  this->_isNumericResponse = false;
  this->_numericResponse = "";
}

Message::Message(const std::string &message) {
  Parser parser(message);
  this->_prefix = parser.getPrefix();
  this->_command = parser.getCommand();
  this->_params = parser.getParams();
  this->_isNumericResponse = false;
  this->_numericResponse = "";
}

Message::Message(
    const std::string prefix,
    MessageConstants::CommandType command,
    const std::vector<std::string> params) {
  this->_prefix = ":" + prefix;
  this->_command = command;
  this->_params = params;
  this->_isNumericResponse = false;
  this->_numericResponse = "";
}

Message::Message(
    const std::string prefix, const int responseCode, const std::vector<std::string> params) {
  this->_prefix = ":" + prefix;
  this->_command = MessageConstants::UNDEFINED;
  this->_params = params;
  this->_isNumericResponse = true;

  std::ostringstream ss;
  ss << responseCode;
  this->_numericResponse = ss.str();
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

const MessageConstants::CommandType &Message::getCommand() const { return this->_command; }

const std::vector<std::string> &Message::getParams() const { return this->_params; }

const std::string &Message::getNumericResponse() const { return this->_numericResponse; }

const bool &Message::isNumericResponse() const { return this->_isNumericResponse; }
