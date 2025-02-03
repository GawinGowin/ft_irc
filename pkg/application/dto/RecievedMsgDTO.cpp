#include "application/dto/RecievedMsgDTO.hpp"

RecievedMsgDTO::RecievedMsgDTO() : _message(""), _senderId(0) {}

RecievedMsgDTO::RecievedMsgDTO(const std::string &message, const int &senderId)
    : _message(message), _senderId(senderId) {}

RecievedMsgDTO::RecievedMsgDTO(const RecievedMsgDTO &obj)
    : _message(obj._message), _senderId(obj._senderId) {}

RecievedMsgDTO &RecievedMsgDTO::operator=(const RecievedMsgDTO &obj) {
  this->_message = obj._message;
  this->_senderId = obj._senderId;
  return (*this);
}

const std::string &RecievedMsgDTO::getMessage() const { return (_message); }

const int &RecievedMsgDTO::getSenderId() const { return (_senderId); }
