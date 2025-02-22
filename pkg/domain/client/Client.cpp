#include "domain/client/Client.hpp"

Client::Client() : _id(0), _socketFd(0), _connectionInfo() {}

Client::Client(int id, pollfd pollfd, const ConnectionInfo connectionInfo)
    : _id(id), _socketFd(pollfd.fd), _pollfd(pollfd), _connectionInfo(connectionInfo) {}

Client::~Client() {}

Client::Client(const Client &other) { *this = other; }

Client &Client::operator=(const Client &other) {
  if (this != &other) {
    this->_id = other._id;
    this->_socketFd = other._socketFd;
    this->_pollfd = other._pollfd;
    this->_connectionInfo = other._connectionInfo;
  }
  return *this;
}

Client *Client::clone() const { return new Client(*this); }

const int &Client::getId() const { return this->_id; }

const int &Client::getSocketFd() const { return this->_socketFd; }

const pollfd &Client::getPollfd() const { return this->_pollfd; }

bool Client::operator==(const IClientAggregateRoot &other) const {
  const Client *otherClient = dynamic_cast<const Client *>(&other);
  if (otherClient == NULL) {
    return false;
  }
  return this->_id == otherClient->_id && this->_socketFd == otherClient->_socketFd;
}

void Client::setConnectionInfo(const ConnectionInfo &connectionInfo) {
  this->_connectionInfo = connectionInfo;
}
const std::string &Client::getNickName() const { return this->_nickName; }

void Client::setNickName(const std::string &nickName) { this->_nickName = nickName; }
