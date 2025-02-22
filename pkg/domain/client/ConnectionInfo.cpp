#include "domain/client/ConnectionInfo.hpp"

ConnectionInfo::ConnectionInfo() : _sock(-1), _addr(""), _pollfd() {}

ConnectionInfo::ConnectionInfo(std::string addr, pollfd pollfd)
    : _sock(pollfd.fd), _addr(addr), _pollfd(pollfd) {}

ConnectionInfo::~ConnectionInfo() {}

ConnectionInfo::ConnectionInfo(const ConnectionInfo &other) { *this = other; }

ConnectionInfo &ConnectionInfo::operator=(const ConnectionInfo &other) {
  if (this != &other) {
    this->_sock = other._sock;
    this->_addr = other._addr;
    this->_pollfd = other._pollfd;
  }
  return *this;
}

bool ConnectionInfo::operator==(const ConnectionInfo &other) const {
  return (
      this->_addr == other._addr && this->_pollfd.events == other._pollfd.events &&
      this->_pollfd.fd == other._pollfd.fd && this->_pollfd.revents == other._pollfd.revents);
}

const int &ConnectionInfo::getSocketFd() const { return this->_sock; }

const std::string &ConnectionInfo::getAddress() const { return this->_addr; }

const pollfd &ConnectionInfo::getPollfd() const { return this->_pollfd; }
