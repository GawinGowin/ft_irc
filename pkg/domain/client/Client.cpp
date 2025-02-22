#include "domain/client/Client.hpp"

Client::Client() : _id(0), _nickName(""), _password(""), _connectionInfo() {}

Client::Client(std::string addr, pollfd pollfd)
    : _id(0), _nickName(""), _password(""), _connectionInfo(ConnectionInfo(addr, pollfd)) {}

Client::~Client() {}

Client::Client(const Client &other) { *this = other; }

Client &Client::operator=(const Client &other) {
  if (this != &other) {
    this->_id = other._id;
    this->_nickName = other._nickName;
    this->_password = other._password;
    this->_connectionInfo = other._connectionInfo;
  }
  return *this;
}

Client *Client::clone() const { return new Client(*this); }

const int &Client::getSocketFd() const { return this->_connectionInfo.getSocketFd(); }

const pollfd &Client::getPollfd() const { return this->_connectionInfo.getPollfd(); }

bool Client::operator==(const IClientAggregateRoot &other) const {
  const Client *otherClient = dynamic_cast<const Client *>(&other);
  if (otherClient == NULL) {
    return false;
  }
  return this->_id == otherClient->_id && this->_nickName == otherClient->_nickName &&
         this->_password == otherClient->_password &&
         this->_connectionInfo == otherClient->_connectionInfo;
}

const int &Client::getId() const { return this->_id; }

const std::string &Client::getNickName() const { return this->_nickName; }

const std::string &Client::getPassword() const { return this->_password; }

void Client::setId(const int &id) { this->_id = id; }

void Client::setNickName(const std::string &nickName) { this->_nickName = nickName; }

void Client::setPassword(const std::string &password) { this->_password = password; }
