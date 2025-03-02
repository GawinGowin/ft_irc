#include "application/dto/SendMsgDTO.hpp"

SendMsgDTO::SendMsgDTO() : _status(0), _messageStreams(MessageStreamVector()) {}

SendMsgDTO::SendMsgDTO(int status, MessageStreamVector &messageStreams)
    : _status(status), _messageStreams(messageStreams) {}

SendMsgDTO::~SendMsgDTO() {}

SendMsgDTO::SendMsgDTO(const SendMsgDTO &obj) { *this = obj; }

SendMsgDTO &SendMsgDTO::operator=(const SendMsgDTO &obj) {
  if (this != &obj) {
    this->_messageStreams = obj._messageStreams;
    this->_status = obj._status;
  }
  return *this;
}

const int &SendMsgDTO::getStatus() const { return _status; }

void SendMsgDTO::setStatus(int status) { this->_status = status; }

MessageStreamVector &SendMsgDTO::getMessageStreams() { return this->_messageStreams; }

void SendMsgDTO::setMessageStreams(const MessageStreamVector &message) {
  this->_messageStreams = message;
}