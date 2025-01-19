#include "ServerConnection.hpp"

ServerConnection::ServerConnection(const std::string &addr, const int &port)
    : watchingAddress(0), clientSocket(0), assignedAddress({}) {
  this->watchingAddress = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (this->watchingAddress == -1) {
    throw std::runtime_error("socket() failed");
  }
  this->assignedAddress.sin_family = AF_INET;
  this->assignedAddress.sin_port = htons((unsigned short)port);
  this->assignedAddress.sin_addr.s_addr = inet_addr(addr.c_str());

  const struct sockaddr *const_assignedAddress =
      reinterpret_cast<const struct sockaddr *>(&this->assignedAddress);
  if (bind(this->watchingAddress, const_assignedAddress, sizeof(this->assignedAddress)) == -1) {
    close(this->watchingAddress);
    throw std::runtime_error("bind() failed");
  }
  if (listen(this->watchingAddress, SOMAXCONN) == -1) {
    close(this->watchingAddress);
    throw std::runtime_error("listen() failed");
  }
}

ServerConnection::~ServerConnection() { close(this->watchingAddress); }

const int &ServerConnection::getWatchingAddress() const { return this->watchingAddress; }

const int &ServerConnection::getClientSocket() const { return this->clientSocket; }

const struct sockaddr_in &ServerConnection::getAssignedAddress() const {
  return this->assignedAddress;
}
