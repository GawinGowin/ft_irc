#include "domain/client/ConnectionInfo.hpp"

ConnectionInfo::ConnectionInfo() : _sock(-1), _addr(""), _pwd("") {}

ConnectionInfo::ConnectionInfo(int sock, std::string addr) : _sock(sock), _addr(addr), _pwd("") {}

ConnectionInfo::~ConnectionInfo() {}

ConnectionInfo::ConnectionInfo(const ConnectionInfo &other) { *this = other; }

ConnectionInfo &ConnectionInfo::operator=(const ConnectionInfo &other) {
  if (this != &other) {
    this->_sock = other._sock;
    this->_addr = other._addr;
    this->_pwd = other._pwd;
  }
  return *this;
}

bool ConnectionInfo::operator==(const ConnectionInfo &other) const {
  return this->_sock == other._sock && this->_addr == other._addr && this->_pwd == other._pwd;
}

const int &ConnectionInfo::getSocketFd() const { return this->_sock; }

const std::string &ConnectionInfo::getAddress() const { return this->_addr; }

const std::string &ConnectionInfo::getPassword() const { return this->_pwd; }
