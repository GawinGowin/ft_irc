#include "domain/client/Client.hpp"

Client::Client() : _id(0), _nickName(""), _userName(""), _password(Password()), _connectionInfo() {}

Client::Client(std::string addr, pollfd pollfd)
    : _id(0), _nickName(""), _userName(""), _password(Password()),
      _connectionInfo(ConnectionInfo(addr, pollfd)) {}

Client::~Client() {}

Client::Client(const Client &other) { *this = other; }

Client &Client::operator=(const Client &other) {
  if (this != &other) {
    this->_id = other._id;
    this->_nickName = other._nickName;
    this->_userName = other._userName;
    this->_password = other._password;
    this->_connectionInfo = other._connectionInfo;
  }
  return *this;
}

bool Client::operator==(const IClientAggregateRoot &other) const {
  const Client *otherClient = dynamic_cast<const Client *>(&other);
  if (otherClient == NULL) {
    return false;
  }
  return this->_id == otherClient->_id && this->_nickName == otherClient->_nickName &&
         this->_userName == otherClient->_userName && this->_password == otherClient->_password &&
         this->_connectionInfo == otherClient->_connectionInfo;
}

Client *Client::clone() const { return new Client(*this); }

const std::string &Client::getId() const { return this->_nickName; } // TODO: id -> nickName

const std::string &Client::getNickName() const { return this->_nickName; }

const std::string &Client::getUserName() const { return this->_userName; }

const std::string &Client::getPassword() const { return this->_password.getPassword(); }

const int &Client::getSocketFd() const { return this->_connectionInfo.getSocketFd(); }

const pollfd &Client::getPollfd() const { return this->_connectionInfo.getPollfd(); }

const std::string &Client::getAddress() const { return this->_connectionInfo.getAddress(); }

void Client::setId(const std::string &id) { this->_nickName = id; } // TODO: id -> nickName

void Client::setNickName(const std::string &nickName) {
  this->_nickName = nickName;
  this->_userName = "~" + nickName;
}

void Client::setUserName(const std::string &userName) { this->_userName = userName; }

int Client::setPassword(const std::string &password) {
  if (this->_password.setPassword(password)) {
    return 1; // password change successful
  }
  return 0; // password change failed
}
