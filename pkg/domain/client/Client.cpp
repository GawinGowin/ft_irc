#include "domain/client/Client.hpp"

Client::Client() : _id(0), _socketFd(0) {}

Client::~Client() {}

Client::Client(const Client &other) { *this = other; }

Client &Client::operator=(const Client &other) {
  if (this != &other) {
    this->_id = other._id;
    this->_socketFd = other._socketFd;
  }
}

const int &Client::getId() const { return this->_id; }

const int &Client::getSocketFd() const { return this->_socketFd; }

bool Client::operator==(const Client &other) const {
  return this->_id == other._id && this->_socketFd == other._socketFd;
}