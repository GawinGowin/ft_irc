#include "application/dto/RecievedMsgDTO.hpp"

RecievedMsgDTO::RecievedMsgDTO() : _message(""), _senderFd(0), _client(NULL) {}

RecievedMsgDTO::RecievedMsgDTO(const std::string &message, IClientAggregateRoot *client)
    : _message(message), _senderFd(client->getSocketFd()), _client(client) {}

RecievedMsgDTO::RecievedMsgDTO(const RecievedMsgDTO &obj) { *this = obj; }

RecievedMsgDTO &RecievedMsgDTO::operator=(const RecievedMsgDTO &obj) {
  if (this != &obj) {
    this->_message = obj._message;
    this->_senderFd = obj._senderFd;
    this->_client = obj._client;
  }
  return (*this);
}

const std::string &RecievedMsgDTO::getMessage() const { return (_message); }

const int &RecievedMsgDTO::getSenderFd() const { return (_senderFd); }

IClientAggregateRoot *RecievedMsgDTO::getClient() const { return this->_client; }