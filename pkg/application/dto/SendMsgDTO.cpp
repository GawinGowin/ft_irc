#include "application/dto/SendMsgDTO.hpp"

std::ostream &operator<<(std::ostream &os, const SendMsgDTO &msgDTO) {
  Message msg = msgDTO.getMessage();
  std::vector<std::string>::const_iterator it;

  os << ":" << msg.getPrefix() << " " << msg.getCommand() << " ";
  for (it = msg.getParams().begin(); it != msg.getParams().end(); ++it) {
    os << *it;
    if (it + 1 != msg.getParams().end()) {
      os << " ";
    }
  }
  return os;
}

SendMsgDTO::SendMsgDTO(const Message &message) : _message(message) {}

SendMsgDTO::~SendMsgDTO() {}

SendMsgDTO::SendMsgDTO(const SendMsgDTO &obj) { *this = obj; }

SendMsgDTO &SendMsgDTO::operator=(const SendMsgDTO &obj) {
  if (this != &obj) {
    this->_message = obj._message;
  }
  return *this;
}

const Message &SendMsgDTO::getMessage() const { return this->_message; }
